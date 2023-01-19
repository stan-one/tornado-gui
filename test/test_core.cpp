#include "../include/core.hpp"
#include "../include/serial_communication.hpp"
#include "../src/shared_data.cpp"
#include "../include/led_effect.hpp"

#include <gtest/gtest.h>

using namespace std;

#define DEF_FQ 20000
#define PWM_FQ 1122
#define RPM 1000
#define TEMP 22.5
#define NUM_STRIP_FAN 0
#define NUM_LED_STRIP 90
#define NUM_LED_FAN 20
#define LED_STRIP_12V 1
#define STRIP_SELECT 0
#define EFFECT 0
#define IMPOSSIBLE_VALUE -1

#define TEST_LED_FAN 1
#define LED_POSITION 3

#define LED_STRING_BEGIN "@12;12;12|"//len 9+1
#define LED_STRIP_SIZE 182 //20*9+2, 20 leds begin char + end char



TEST(led_test, setup_led){
    //setup
    core test_core;
    effects_t selected = RANDOM_COLOR;
    led_effect one_led;
    setup_data_t data = {NUM_LED_FAN, NUM_LED_STRIP, STRIP_SELECT};
    //test
    q_uisetup2core.push(data);
    ASSERT_TRUE(test_core.load_led_core());
    run_effect(RANDOM_COLOR);
    one_led = get_led_fan(TEST_LED_FAN, LED_POSITION);

    ASSERT_EQ(one_led.B, LED_RGB);
    ASSERT_EQ(one_led.R, LED_RGB);
    ASSERT_EQ(one_led.G, LED_RGB);
    //test string
    string hold;
    hold = q_core2serial.front();
    ASSERT_EQ(LED_STRING_BEGIN, hold.substr(0,10));

    //tear down
    queue<string> empty;
    std::swap(q_core2serial, empty );
    ASSERT_EQ(q_core2serial.size(), 0);
}


TEST(core_test, ui2core){
    //setup
    effects_t selected = RANDOM_COLOR;
    datapack_fe_t fe_data = {PWM_FQ, PWM_FQ, PWM_FQ, PWM_FQ, 
                            true, true, LED_STRIP_12V ,selected, 0,DEF_FQ};
    core test_core;
    q_ui2core.push(fe_data);
    //test that the struct converts to the right string
    test_core.create_fe_dp();
    ASSERT_EQ(q_core2serial.size(), 5);//setting + led effect
    ASSERT_EQ(q_core2serial.front(), "$1122;1122;1122;1122;1;1;1;20000#");
    //teardown, simulate that the data has been send to the pcb thru the serial communication thread
    queue<string> empty;
    std::swap(q_core2serial, empty );
    //test that if the same struct is inserted again no string is generated
    q_ui2core.push(fe_data);
    test_core.create_fe_dp();
    ASSERT_EQ(q_core2serial.size(), 0);
    //teardown
    
}

TEST(core_test, serial2core){
    //setup
    datapack_be_t be_data;
    core test_core;
    string data_from_serial = "1001;1002;1003;1004;1005;22.5#";
    q_serial2core.push(data_from_serial);
    //test string from pcb
    ASSERT_EQ(q_serial2core.size(), 1);
    test_core.create_be_dp();
    be_data = q_core2ui.front();
    EXPECT_EQ(be_data.rpm_f1, RPM+1);
    EXPECT_EQ(be_data.rpm_f2, RPM+2);
    EXPECT_EQ(be_data.rpm_f3, RPM+3);
    EXPECT_EQ(be_data.rpm_f4, RPM+4);
    EXPECT_EQ(be_data.rpm_pump, RPM+5);
    EXPECT_EQ(be_data.temperature, TEMP);
    //teardown
    q_core2ui.pop();
}


TEST(core_test, corrupted_data){
    core test_core;
    datapack_be_t be_data;
    string data_from_serial = "1001+1002;1AA3;1004;1005;";
    q_serial2core.push(data_from_serial);
    ASSERT_EQ(q_serial2core.size(), 1);
    test_core.create_be_dp();
    be_data = q_core2ui.front();
    EXPECT_EQ(be_data.rpm_f1, IMPOSSIBLE_VALE);
    EXPECT_EQ(be_data.rpm_f2, IMPOSSIBLE_VALE);
    EXPECT_EQ(be_data.rpm_f3, IMPOSSIBLE_VALE);
    EXPECT_EQ(be_data.rpm_f4, IMPOSSIBLE_VALE);
    EXPECT_EQ(be_data.rpm_pump, IMPOSSIBLE_VALE);
    EXPECT_EQ(be_data.temperature, IMPOSSIBLE_VALE);
}

TEST(core_test, serial_ports){
    //setup
    core test_core;
    //test
    ASSERT_EQ(v_ports.size(), 0);
    test_core.load_serial_ports();
    ASSERT_EQ(v_ports.empty(), false);
    //teardown
    v_ports.clear();
}




int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
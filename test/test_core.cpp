#include "../include/core.hpp"
#include "../include/serial_communication.hpp"
#include "../src/shared_data.cpp"

#include <gtest/gtest.h>

using namespace std;

#define DEF_FQ 20000
#define PWM_FQ 1122
#define RPM 1000
#define TEMP 22.5
#define NUM_STRIP_FAN 0
#define NUM_LED_STRIP 90
#define NUM_LED_FAN 20
#define LED_STRIP 0
#define STRIP_SELECT 0
#define EFFECT 0
TEST(core_test, ui2core){
    //setup
    datapack_fe_t fe_data = {PWM_FQ, PWM_FQ, PWM_FQ, PWM_FQ, 
                            true, true, NUM_LED_FAN, NUM_LED_STRIP, EFFECT,EFFECT,STRIP_SELECT, DEF_FQ};
    core test_core;
    q_ui2core.push(fe_data);
    //test that the struct converts to the right string
    test_core.create_fe_dp();
    ASSERT_EQ(q_core2serial.size(), 1);
    ASSERT_EQ(q_core2serial.front(), "1122;1122;1122;1122;1;1;20;90;0;0;0;20000#");
    q_core2serial.pop();
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
    string data_from_serial = "1000;1000;1000;22.5#";
    q_serial2core.push(data_from_serial);
    //test
    ASSERT_EQ(q_serial2core.size(), 1);
    test_core.create_be_dp();
    be_data = q_core2ui.front();
    EXPECT_EQ(be_data.rpm_f1, RPM);
    EXPECT_EQ(be_data.rpm_f2, RPM);
    EXPECT_EQ(be_data.rpm_f3, RPM);
    EXPECT_EQ(be_data.temperature, TEMP);
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
#include "../include/core.hpp"
#include "../include/serial_communication.hpp"
#include "../src/shared_data.cpp"

#include <gtest/gtest.h>

using namespace std;

#define DEF_FQ 20000
#define PWM_FQ 1122
#define RPM 1000
#define TEMP 22.5

TEST(core_test, ui2core){
    //setup
    datapack_fe_t fe_data = {DEF_FQ, PWM_FQ, PWM_FQ, PWM_FQ};
    core test_core;
    q_ui2core.push(fe_data);
    //test
    test_core.create_fe_dp();
    ASSERT_EQ(q_core2serial.size(), 1);
    ASSERT_EQ(q_core2serial.front(), "20000;1122;1122;1122#");
    //teardown
    q_core2serial.pop();
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
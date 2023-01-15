/**
 * @mainpage Simple 3 fan control UI. Made using ImGui running on multiple threads
 * 
 * @file core.hpp
 * @author your name (you@domain.com)
 * @brief header file for core
 * @version 0.1
 * @date 2023-01-08
 * @author Stanimir Htistov
 * @copyright Copyright (c) 2023
 * @class core Middelware between the front-end and the hardware. Acts as a translation and directioning unit 
 *        so that raw data comming from the UI or from the PCB can be routed
 */

#pragma once

#include <thread>
#include <string>
#include <queue>
#include <chrono>
#include <mutex>
#include <atomic>
#include <iostream>
#include <sstream>

#include "../lib/serial/serial.h"
#include "../include/serial_communication.hpp"

#define TIMEOUT 10 //ms

using namespace std;

#define DEFAULT_HZ 20000
#define MAX_HZ 22000

/**
 * @defgroup datapacks Datapacks
 * @brief structs that represent the information being passed between the UI and thr PCB
 * @{
 */

typedef struct datapack_fd{
    int pwm_f1 = 0;
    int pwm_f2 = 0;
    int pwm_f3 = 0;
    int freq_pwm = DEFAULT_HZ;
}datapack_fe_t;

typedef struct datapack_be{
    int rpm_f1 = 0;
    int rpm_f2 = 0;
    int rpm_f3 = 0;
    float temperature = 0;
}datapack_be_t;

/**
 * @}
 */
class core{
    public:
    core();
    void run();
    void load_serial_ports();
    
    private:
    /**
     * @brief Create a frond end datapack object
     * 
     */
    void create_fe_dp();
    /**
     * @brief Create a back end datapack object
     * 
     */
    void create_be_dp();    
    /**
     * @brief Local datapacks. Used as storage for the current state of the app
     * @{
     */
    datapack_fe_t core_fe;
    datapack_be_t core_be;
    /**
     * @}
    */

};


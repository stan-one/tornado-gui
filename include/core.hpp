
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
#define MAX_LEDS_FAN 50
#define MAX_LEDS_STRIP 200
#define IMPOSSIBLE_VALE -1
#define INCOMING_PARAMS 6

#define TESTING_CORE true

#define SETUP_COMMAND "&"
#define UI_COMMAND "$"
#define LED_COMMAND "@"

#define SEPARATOR_STR ";"
#define SEPARATOR ';'

enum effects{STATIC_COLOR, RANDOM_COLOR};


/**
 * @defgroup datapacks Datapacks
 * @brief structs that represent the information being passed between the UI and thr PCB
 * @{
 */

typedef struct datapack_fd{
    int pwm_f1 = 0;
    int pwm_f2 = 0;
    int pwm_f3 = 0;
    int pwm_f4 = 0;
    bool sw_1 = 0;
    bool sw_2 = 0;
    int effect_selected_fan;
    int effect_selected_strip = 0;
    int freq_pwm = DEFAULT_HZ;
}datapack_fe_t;

typedef struct setup_data{
    int num_leds_fan = 0;
    int num_leds_strip = 0;
}setup_data_t;

typedef struct datapack_be{
    int rpm_f1 = 0;
    int rpm_f2 = 0;
    int rpm_f3 = 0;
    int rpm_f4 = 0;
    int rpm_pump = 0;
    float temperature = 0;
}datapack_be_t;

/**
 * @}
 */

/**
 * @brief Processes and passes informarion from the front UI to the PCB.
 * 
 */
class core{
    public:
    /**
     * @brief Construct a new core object
     * 
     */
    core();
    /**
     * @brief Before the startup all avalible serial ports are listed. From the UI a user selects.
     * 
     */
    void load_serial_ports();
    /**
     * @brief Configures the datastructures needed for the led effects
     * 
     * @return true 
     * @return false 
     */
    bool load_led_core();

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
    private:
    datapack_fe_t core_fe;
    datapack_be_t core_be;
    int current_effect;
    /**
     * @}
    */

};


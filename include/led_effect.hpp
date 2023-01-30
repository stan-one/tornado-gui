#pragma once

#include <vector>
#include <cinttypes>
#include <string>
#include <random>
#include "core.hpp"

#define TIMESTEP_LED 200
#define NUM_FANS 4
#define SEPARATOR_LED "|"
#define END_CHAR "#"

#define TESTING_LED true
#define LED_RGB 12

using namespace std;

/**
 * @brief The datatype that represents every smart led
 * 
 */
typedef struct led_effect
{
    led_effect(){}
    int R = 0;
    int G = 0;
    int B = 0;
}led_color_t;

extern queue<string> q_core2serial;  
extern mutex m_core2serial;
/**
 * @brief Initializes the data structures that represent represent the leds in the app
 * 
 * @param num_led_fan 
 * @param num_led_strip 
 */
void init_led_vector(int num_led_fan, int num_led_strip);
/**
 * @brief Returns the state of a single led
 * 
 * @param num_fan 
 * @param num_led 
 * @return led_color_t 
 */
led_color_t get_led_fan(int num_fan, int num_led);
/**
 * @brief Selects the effect to run
 * @todo Add more effects 
 * @param eff 
 */
void run_effect(int eff);
/**
 * @brief Generates the data needed for the specific effect
 * @note the effect is generated for all 4 fans at once
 * @warning Is not really pulsating
 * 
 * @return vector<string>& 
 */
vector<string> &pulsating_random_color();


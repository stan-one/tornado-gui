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

typedef struct led_effect
{
    led_effect(){}
    int R = 0;
    int G = 0;
    int B = 0;
}led_color_t;

extern queue<string> q_core2serial;  
extern mutex m_core2serial;



void init_led_vector(int num_led_fan, int num_led_strip);
led_color_t get_led_fan(int num_fan, int num_led);
void run_effect(int eff);
vector<string> &pulsating_random_color();


#pragma once

#include <vector>
#include <cinttypes>
#include <string>
#include <random>

#define TIMESTEP_LED 200
#define NUM_FANS 4

using namespace std;

int num_led_fan = 0;
int num_led_strip = 0;

typedef struct led_effect
{
    led_effect(){}
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
}led_effect_t;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(0,255);

vector<vector<led_effect_t>> led_fans;
vector<led_effect_t> led_strip;

void init_led_vector();
void run_effect(int effect);
vector<vector<string>> &pulsating_random_color(uint steps);


#include "../include/led_effect.hpp"


void init_led_vector(){
    led_fans = vector<vector<led_effect>>(NUM_FANS, 
    vector<led_effect_t>(num_led_fan, led_effect_t()));
    led_strip = vector<led_effect_t>(num_led_strip, led_effect_t());
}

vector<vector<string>> &pulsating_random_color(uint steps){
    static vector<vector<string>> effects;
    led_effect_t led;
    led_fans.at(0).at(0).B;
    for(auto it_fans = led_fans.begin(); it_fans != led_fans.end(); ++it_fans){
        int R = dist(rng); int G = dist(rng); int B = dist(rng);
        for(auto it_fan = it_fans->begin(); it_fan != it_fans->end(); ++it_fan){
            it_fan->R = R; it_fan->B = B; it_fan->R = R; 
        }
    }

    for(auto it_fans = led_fans.begin(); it_fans != led_fans.end(); ++it_fans){
        for(auto it_fan = it_fans->begin(); it_fan != it_fans->end(); ++it_fan){
            it_fan->R; it_fan->B; it_fan->R;
        }
    }

}
#include "../include/led_effect.hpp"


std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(0,255);

vector<vector<led_color_t>> led_fans;
vector<led_color_t> led_strip;


void init_led_vector(int num_led_fan, int num_led_strip){
    led_fans = vector<vector<led_effect>>(NUM_FANS, 
    vector<led_color_t>(num_led_fan, led_color_t()));
    led_strip = vector<led_color_t>(num_led_strip, led_color_t());
}

vector<string> &pulsating_random_color(){
   static vector<string> effects;

    led_color_t led;
    for(auto it_fans = led_fans.begin(); it_fans != led_fans.end(); ++it_fans){
        #if TEST == true
            int R = LED_RGB; int G = LED_RGB; int B = LED_RGB;
        #else
            int R = dist(rng); int G = dist(rng); int B = dist(rng);
        #endif
        for(auto it_fan = it_fans->begin(); it_fan != it_fans->end(); ++it_fan){
            it_fan->R = R; it_fan->B = B; it_fan->G = G; 
        }
    }
    string hold;
    hold.append(LED_COMMAND);
    for(auto it_fans = led_fans.begin(); it_fans != led_fans.end(); ++it_fans){
        for(auto it_fan = it_fans->begin(); it_fan != it_fans->end(); ++it_fan){
            hold.append(to_string(it_fan->G)); hold.append(SEPARATOR_COLOR);
            hold.append(to_string(it_fan->R)); hold.append(SEPARATOR_COLOR);
            hold.append(to_string(it_fan->B));
            
            hold.append(SEPARATOR_LED);
        }
        hold.append(END_CHAR);
        effects.push_back(hold);
    }
    return effects;
}


void run_effect(effects_t eff){
    switch (eff)
    {
    case RANDOM_COLOR:
        {
            lock_guard<mutex> lk(m_core2serial);
            q_core2serial.push(pulsating_random_color().at(0));
            q_core2serial.push(pulsating_random_color().at(1));
            q_core2serial.push(pulsating_random_color().at(2));
            q_core2serial.push(pulsating_random_color().at(3));
        }
        
        break;
    
    default:
        break;
    }
}

led_color_t get_led_fan(int num_fan, int num_led){
    return led_fans.at(num_fan).at(num_led);
}
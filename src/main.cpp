#include "../include/render.hpp"
#include "../include/core.hpp"
#include "../include/serial_communication.hpp"
#include <thread>

extern bool run_gui;
extern vector<string> v_ports;
extern atomic<int> port_num;
extern atomic<bool> setup;


void gui(){
    graphics control_panel(720, 600);
    while(true){
        control_panel.prereder();
        if(control_panel.deploy_ui()){
            break;
        }
        control_panel.render();
    }
    control_panel.close();
}

void run_core(){
    core app_engine;
    app_engine.load_serial_ports();
    while(!setup){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    app_engine.load_led_core();
    while(run_gui){
        app_engine.create_fe_dp();
        app_engine.create_be_dp();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void run_serial_communication(){
    
    while(port_num==0){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    serial_communication serial_pcb(v_ports.at(port_num),9600, serial::Timeout(2000));
    while(run_gui){
        serial_pcb.serial_send_pcb();
        serial_pcb.serial_receive_pcb();
        serial_pcb.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main(){

    thread engine_th(run_core);
    thread gui_th(gui);
    thread serial(run_serial_communication);
    
    gui_th.join();
    serial.join();
    engine_th.join();
    return 0;
}
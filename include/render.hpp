#pragma once

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 
#include <stdexcept>

#include <string>
#include <iostream>

#include "../include/core.hpp"

extern queue<datapack_fe_t> q_ui2core;  extern mutex m_ui2core; 
extern queue<datapack_be_t> q_core2ui;  extern mutex m_core2ui; 
extern queue<setup_data_t> q_uisetup2core; 
extern std::chrono::milliseconds timeout;
extern string port_list;
extern bool port_set_flag;
extern atomic<int> port_num;
extern atomic<bool> setup;
extern  bool operator==(const datapack_fd& lhs, const datapack_fd& rhs);

class graphics{
    public:
    graphics(int win_h_in, int win_w_in);
    void prereder();
    bool deploy_ui();
    void render();
    bool close();
    private:
    GLFWwindow* window{NULL};  
    ImGuiIO* io;
    ImGuiStyle* style;
    int hz_pwm_fan{DEFAULT_HZ};
    const char* glsl_version = "#version 130";
    datapack_fe_t ui_fe; 
    datapack_be_t ui_be;
    setup_data_t setup_data;
};



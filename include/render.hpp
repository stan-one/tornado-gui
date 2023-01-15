#pragma once

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 
#include <stdexcept>

#include <string>
#include <iostream>

#include "../include/core.hpp"

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
};



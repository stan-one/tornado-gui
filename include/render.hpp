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

/**
 * @brief A wrapper class around the imgui library. Improves ease of use.
 * 
 */
class graphics{
    public:
    /**
     * @brief Construct a new graphics object.
     * @todo At start up appears an ugly black window, find a way to remove it.
     * @param win_h_in window hight
     * @param win_w_in window with
     */
    graphics(int win_h_in, int win_w_in);
    /**
     * @brief Pick up the events and prepare the next frame
     * 
     */
    void prereder();
    /**
     * @brief Represents the UI. if it returns false then the UI has to close
     * 
     * @return true 
     * @return false 
     */
    bool deploy_ui();
    /**
     * @brief Deploys the configuration data to the user
     * 
     */
    void render();
    /**
     * @brief destroys all the allocated objetcs
     * @todo make this method the destructor of the class, this makes no sense
     * 
     * @return true 
     * @return false 
     */
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



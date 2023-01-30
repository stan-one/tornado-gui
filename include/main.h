#ifndef MAIN_H
#define MAIN_H

#include "../include/render.hpp"
#include "../include/core.hpp"
#include "../include/serial_communication.hpp"
#include <thread>

/**
 * @brief The GUI thread. Contains the imgui library running
 * 
 */
void gui();
/**
 * @brief The core thread.
 * 
 */
void run_core();
/**
 * @brief The serial communication thread.
 * 
 */
void run_serial_communication();
#endif

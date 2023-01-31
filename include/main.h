#ifndef MAIN_H
#define MAIN_H

/**
 * @mainpage General description of the app
 * \section descrition Description
 * This software is a multithread app executing 3 individual threads. One for the GUI, one for the core and one for serial communication.
 * At startup, the number of leds of the fan and the ~~strip~~ are selected and the serial port from which data will be sent and retrieve from the PCB
 * Once this is done the app starts and continuously exchanges data between threads.
 * 
 * For the GUI was used ImGui, and for the serial communication a library provided by  William Woodall, John Harrison was used. For the multithreading the C++11
 * standard was used.
 * \section arq Architeture
 * This app is based on the Actor Model architecture. The threads run separately and only exchange data thru thread-safe data structures, in this case queues+mutex and 
 * atomic variables. In the Bridge Data module shows all the used structures. The naming system used is "y_a2b"; 'y' is for the datatype q (queue) a is the thread that 
 * puts data in the data structure, b is the thread that retrieves data from the data structure.
 * 
 * | UI |           CORE | SERIAL  |
| ---- |           ---- | ---- |
| read user input    | process ui data     | send command    |
| update ui         | process pcb data   | read input  |
 *@warning The hardware is designed to handle 4 rgb fans and 2 led strips; one 12V and the other 5V. Due to a hardware limitation of the microcontroller only the 12V strip is avalible
 *@todo Add effects and support for the led strip
 *  @file main.hpp
 * @author Stanimir Hristov (stanhristovhristov@gmai.com)
 * @brief header file for main
 * @version 0.1
 * @date 2023-01-08
 * @author Stanimir Hristov
 * @copyright Copyright (c) 2023
 * @class core Middelware between the front-end and the hardware. Acts as a translation and directioning unit 
 *        so that raw data comming from the UI or from the PCB can be routed
 */


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

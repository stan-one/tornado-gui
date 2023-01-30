#pragma once

#include <string>
#include <iostream>
#include <cstdio>

#include <thread>
#include <string>
#include <queue>
#include <chrono>
#include <mutex>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../lib/serial/serial.h"
#include "../include/core.hpp"

#define END_OF_STRING "#"
#define END_OF_CHAR '#'
#define NO_SELECTED "---"
using namespace std;

/**
 * @brief Serial comunication class. Reads the data comming from the PCB and sends it to core
 * also, sends data from the core to the PCB
 * 
 */
class serial_communication: public serial::Serial{
    public:
    /**
     * @brief Construct a new serial communication object. Depends on an external binary
     * 
     * @param port 
     * @param baud_rate 
     * @param serial_timeout 
     */
    serial_communication(string port, int baud_rate, serial::Timeout serial_timeout);
    /**
     * @brief Send data to the pcb. Reads from queue.
     * 
     */
    void serial_send_pcb();
    /**
     * @brief Receives data from pcb. Writes to queue.
     * 
     */

    void serial_receive_pcb();

    private:
    string pcb2serial;
    string serial2pcb;

};

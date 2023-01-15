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
#define SEPARATOR ';'
#define NO_SELECTED "---"

using namespace std;


class serial_communication: public serial::Serial{
    public:
    serial_communication(string port, int baud_rate, serial::Timeout serial_timeout);

    void serial_send_pcb();

    void serial_receive_pcb();

    private:
    string pcb2serial;
    string serial2pcb;

};

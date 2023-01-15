#include "../include/serial_communication.hpp"

using namespace std;
extern queue<string> q_serial2core; extern mutex m_serial2core;
extern queue<string> q_core2serial; extern mutex m_core2serial;
extern queue<string> q_ports; 

//    serial::Serial(port, baud_rate, serial_timeout);

serial_communication::serial_communication(string port, int baud_rate, serial::Timeout serial_timeout):
    serial::Serial(port, baud_rate, serial_timeout){
    if(!this->isOpen()){
        throw std::runtime_error("Serial port unavalible");
    }
};

void serial_communication::serial_send_pcb(){
    {
        lock_guard<mutex> lk(m_core2serial);
        if(!q_core2serial.empty()){
            //cout<<"data in q_size: "<<q_core2serial.size()<<endl;
            serial2pcb = q_core2serial.front();
            q_core2serial.pop();
            this->write(serial2pcb);
        }
    }
}

void serial_communication::serial_receive_pcb(){
    pcb2serial = this->readline(1024, string(END_OF_STRING));
    //cout<<"received: "<<pcb2serial<<endl;
    if(!pcb2serial.empty()){
        lock_guard<mutex> lk(m_serial2core);
        if(q_serial2core.size()<3){
            q_serial2core.push(pcb2serial);
        }
    }
}

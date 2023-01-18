
#include "../include/core.hpp"

using namespace std;

queue<datapack_fe_t> q_ui2core;   mutex m_ui2core; 
queue<datapack_be_t> q_core2ui;   mutex m_core2ui; 
queue<setup_data_t> q_uisetup2core; 
queue<string> q_serial2core;  mutex m_serial2core;
queue<string> q_core2serial;  mutex m_core2serial;
vector<string> v_ports; 
atomic<int> port_num{0};
atomic<bool> setup{false};
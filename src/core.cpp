#include "../include/core.hpp"
#include "../include/led_effect.hpp"


using namespace std;

 std::chrono::milliseconds  timeout = chrono::milliseconds(TIMEOUT);

string port_list;

extern queue<datapack_fe_t> q_ui2core;  extern mutex m_ui2core; 
extern queue<datapack_be_t> q_core2ui;  extern mutex m_core2ui; 
extern queue<string> q_serial2core; extern mutex m_serial2core;
extern queue<string> q_core2serial; extern mutex m_core2serial;
extern queue<setup_data_t> q_uisetup2core;
extern vector<string> v_ports; 
extern atomic<int> port_num;

 bool operator==(const datapack_fd& lhs, const datapack_fd& rhs)
{
    return lhs.freq_pwm == rhs.freq_pwm &&
           lhs.pwm_f1 == rhs.pwm_f1 &&
           lhs.pwm_f2 == rhs.pwm_f2 &&
           lhs.pwm_f3 == rhs.pwm_f3 &&
           lhs.pwm_f4 == rhs.pwm_f4 &&
           lhs.sw_1 == rhs.sw_1 &&
           lhs.sw_2 == rhs.sw_2 &&
           lhs.freq_pwm == rhs.freq_pwm;
}

bool port_set_flag{false};


core::core(){}


bool core::load_led_core(){
    if(q_uisetup2core.size()<0){
        return false;
    }
    setup_data_t set = q_uisetup2core.front();
    init_led_vector(set.num_leds_fan, set.num_leds_strip);
    {
        string setup = SETUP_COMMAND + to_string(set.num_leds_fan) + SEPARATOR +  to_string(set.num_leds_strip) + END_CHAR;
        lock_guard<mutex> lk(m_core2serial);
        q_core2serial.push(setup);
    }
    return true;
}


 void core::create_be_dp(){
    string hold_res;
    {
        lock_guard<mutex> lk(m_serial2core);
        if(!q_serial2core.empty()){
            hold_res = q_serial2core.front();
            q_serial2core.pop();
        }
    }

    if(!hold_res.empty()){
        stringstream ss(hold_res);
        vector<string> result;
        while( ss.good() ){
            string substr; 
            
            getline( ss, substr, ';' );
            result.push_back( substr );
        }
        if(result.size()!=INCOMING_PARAMS){
            core_be.rpm_f1 = IMPOSSIBLE_VALE;
            core_be.rpm_f2 = IMPOSSIBLE_VALE;
            core_be.rpm_f3 = IMPOSSIBLE_VALE;
            core_be.rpm_f4 = IMPOSSIBLE_VALE;
            core_be.rpm_pump = IMPOSSIBLE_VALE;
            core_be.temperature = IMPOSSIBLE_VALE;
        }
        else{
        core_be.rpm_f1 = stoi(result.at(0));
        core_be.rpm_f2 = stoi(result.at(1));
        core_be.rpm_f3 = stoi(result.at(2));
        core_be.rpm_f4 = stoi(result.at(3));
        core_be.rpm_pump = stoi(result.at(4));
        core_be.temperature = ((float)stoi(result.at(5)))/100;
        }

    }

    {
        lock_guard<mutex> lk(m_core2ui);
            if(q_core2ui.size()<10){
                q_core2ui.push(core_be);
            }
    }
 }

 void core::create_fe_dp(){
    static datapack_fe_t fe_ans;
    {
        lock_guard<mutex> lk(m_ui2core);
            if(!q_ui2core.empty()){
                core_fe = q_ui2core.front();
                q_ui2core.pop();
            }
    }
    if(!(fe_ans == core_fe)){
        fe_ans = core_fe;
        string to_serial;
        to_serial.append(UI_COMMAND);
        to_serial.append(to_string(core_fe.pwm_f1));
        to_serial.push_back(SEPARATOR);
        to_serial.append(to_string(core_fe.pwm_f2));
        to_serial.push_back(SEPARATOR);
        to_serial.append(to_string(core_fe.pwm_f3));
        to_serial.push_back(SEPARATOR);
        to_serial.append(to_string(core_fe.pwm_f4));
        to_serial.push_back(SEPARATOR);

        core_fe.sw_1 ? to_serial.append("1") : to_serial.append("0"); 
        to_serial.push_back(SEPARATOR);
        core_fe.sw_2 ? to_serial.append("1") : to_serial.append("0"); 
        to_serial.push_back(SEPARATOR);

        to_serial.append(to_string(core_fe.freq_pwm));
        to_serial.push_back(END_OF_CHAR);
        {
            lock_guard<mutex> lk(m_core2serial);
                if(q_core2serial.size()<4){
                    q_core2serial.push(to_serial);
                    to_serial.clear();
                }
        }
        if(current_effect != core_fe.effect_selected_fan){
            current_effect = core_fe.effect_selected_fan;
            run_effect(core_fe.effect_selected_fan);//has access to q_core2serial, mutex inside function
        }

    }
 }

 void core::load_serial_ports(){
    vector<serial::PortInfo> devices_found = serial::list_ports();

	vector<serial::PortInfo>::iterator iter = devices_found.begin();
    
    string hold;
    #if TESTING_CORE == true
    v_ports.push_back(string("---"));
    v_ports.push_back(string("/dev/pts/1"));
    #endif
    port_list.append(NO_SELECTED);
    port_list.push_back('\0');
    #if TESTING_CORE == true
    port_list.append("/dev/pts/1"); port_list.push_back('\0');
    #endif
	while( iter != devices_found.end() ){
		serial::PortInfo device = *iter++;
            hold = device.port;
            hold.push_back('\0');
            port_list.append(hold);
            v_ports.push_back(hold);
            hold.clear();
	}
 }


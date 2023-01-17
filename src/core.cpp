#include "../include/core.hpp"

using namespace std;

 std::chrono::milliseconds  timeout = chrono::milliseconds(TIMEOUT);

string port_list=NO_SELECTED;

 bool operator==(const datapack_fd& lhs, const datapack_fd& rhs)
{
    return lhs.freq_pwm == rhs.freq_pwm &&
           lhs.pwm_f1 == rhs.pwm_f1 &&
           lhs.pwm_f2 == rhs.pwm_f2 &&
           lhs.pwm_f3 == rhs.pwm_f3 &&
           lhs.pwm_f4 == rhs.pwm_f4 &&
           lhs.sw_1 == rhs.sw_1 &&
           lhs.sw_2 == rhs.sw_2 &&
           lhs.num_leds_fan == rhs.num_leds_fan &&
           lhs.num_leds_strip == rhs.num_leds_strip &&
           lhs.strip_select == rhs.strip_select &&
           lhs.freq_pwm == rhs.freq_pwm;
}

bool port_set_flag{false};


core::core(){}

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
        core_be.temperature = stof(result.at(5));
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

        to_serial.append(to_string(core_fe.num_leds_fan));
        to_serial.push_back(SEPARATOR);
        to_serial.append(to_string(core_fe.num_leds_strip));
        to_serial.push_back(SEPARATOR);

        to_serial.append(to_string(core_fe.effect_selected_fan));
        to_serial.push_back(SEPARATOR);
        to_serial.append(to_string(core_fe.effect_selected_strip));
        to_serial.push_back(SEPARATOR);

        core_fe.strip_select ? to_serial.append("1") :to_serial.append("0");
        to_serial.push_back(SEPARATOR);

        to_serial.append(to_string(core_fe.freq_pwm));
        to_serial.push_back(END_OF_CHAR);
        {
            lock_guard<mutex> lk(m_core2serial);
                if(q_core2serial.size()<3){
                    q_core2serial.push(to_serial);
                    to_serial.clear();
                }
        }
    }
 }

 void core::load_serial_ports(){
    vector<serial::PortInfo> devices_found = serial::list_ports();

	vector<serial::PortInfo>::iterator iter = devices_found.begin();
    
    string hold;
    v_ports.push_back(string("---"));
    v_ports.push_back(string("/dev/pts/4"));
    port_list.push_back('\0');
    port_list.append("/dev/pts/4"); port_list.push_back('\0');
	while( iter != devices_found.end() ){
		serial::PortInfo device = *iter++;
            hold = device.port;
            hold.push_back('\0');
            port_list.append(hold);
            v_ports.push_back(hold);
            hold.clear();
	}
 }


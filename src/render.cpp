#include "../include/render.hpp"
#include "../include/core.hpp"
#include "../include/led_effect.hpp"

bool run_gui{true};



graphics::graphics(int win_h, int win_w){
    if (!glfwInit()){
        throw std::runtime_error("COULD NOT INITILIZE GLFW");
    }
    else{
        //set up glfw version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        window = glfwCreateWindow(win_w, win_h, "PROJECT TORNADO: FAN CONTROL", NULL, NULL);
        if (window == NULL){
            throw std::runtime_error("COULD NOT CREATE WINDOW");
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO(); (void)io;
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        ImGui::StyleColorsDark();

        style = &ImGui::GetStyle();
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        style->WindowRounding = 0.0f;
        style->Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
}

void graphics::prereder(){
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

bool graphics::deploy_ui(){
   if(glfwWindowShouldClose(window)){
    run_gui = false;
    return true;
   }
    if(!port_set_flag){
        static int port_local{0};
        ImGui::Begin("Setup Panel");
        ImGui::Text("Select serial port:");
        ImGui::Combo("detected ports: ", &port_local, port_list.c_str());

        ImGui::InputInt("Number LEDs Fan", &setup_data.num_leds_fan, 1, MAX_LEDS_FAN);
        if(setup_data.num_leds_fan>=MAX_LEDS_FAN){
            setup_data.num_leds_fan=MAX_LEDS_FAN;
        }
        else if(setup_data.num_leds_fan<=0){
            setup_data.num_leds_fan=0;
        }
        
        ImGui::InputInt("Number LEDs Strip", &setup_data.num_leds_strip, 1, MAX_LEDS_STRIP);
        if(setup_data.num_leds_strip>=MAX_LEDS_STRIP){
            setup_data.num_leds_strip=MAX_LEDS_STRIP;
        }
        else if(setup_data.num_leds_strip<=0){
            setup_data.num_leds_strip=0;
        }

        port_num = port_local;
        if(ImGui::Button("SAVE")){
            q_uisetup2core.push(setup_data);
            setup = true;
            port_set_flag = true;
        }
        ImGui::End();
    }
    else{
        static datapack_fe_t ui_fe_ans;
        {
        lock_guard<mutex> lk(m_core2ui);
            if(!q_core2ui.empty()){
                ui_be = q_core2ui.front();
                q_core2ui.pop();
            }
        }
        ImGui::Begin("Fan control and LED control panel");

    
        ImGui::SliderInt("FAN 1", &ui_fe.pwm_f1, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f1);

        ImGui::SliderInt("FAN 2", &ui_fe.pwm_f2, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f2);

        ImGui::SliderInt("FAN 3", &ui_fe.pwm_f3, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f3);

        ImGui::SliderInt("FAN 4", &ui_fe.pwm_f4, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f4);

        ImGui::Text("RPM PUMP: %d", ui_be.rpm_pump);

        ImGui::Combo("LED FAN effects", &ui_fe.effect_selected_fan, "EFFECT CC\0EFFECT DD\0");   

        ImGui::Combo("LED Strip effects", &ui_fe.effect_selected_strip, "EFFECT AA\0EFFECT BB\0");   
        ImGui::Checkbox("Switch element 1", &ui_fe.sw_1);
        ImGui::SameLine(); 
        ImGui::Checkbox("Switch element 2", &ui_fe.sw_2);
        ImGui::InputInt("FAN Hz", &ui_fe.freq_pwm, 100, 500);
        if(ui_fe.freq_pwm>=MAX_HZ){
            ui_fe.freq_pwm = MAX_HZ;    
        }
        else if(ui_fe.freq_pwm<=0){
            ui_fe.freq_pwm = 0; 
        }
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),"TEMPERATURE: %.1f", ui_be.temperature);
        {
            lock_guard<mutex> lk(m_ui2core);
                if(q_ui2core.size()<3 && !(ui_fe==ui_fe_ans)){
                    ui_fe_ans = ui_fe;
                    q_ui2core.push(ui_fe);
                }
        }
        ImGui::End();
        return false;
    }
    return false;
}

void graphics::render(){
    int win_w, win_h;
    ImGui::Render();
    glfwGetFramebufferSize(window, &win_w, &win_h);
    glViewport(0, 0, win_w, win_h);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window);
}

bool graphics::close(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return true;
}
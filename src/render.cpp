#include "../include/render.hpp"
#include "../include/core.hpp"


extern queue<datapack_fe_t> q_ui2core;  extern mutex m_ui2core; 
extern queue<datapack_be_t> q_core2ui;  extern mutex m_core2ui; 
extern std::chrono::milliseconds timeout;
extern string port_list;
extern bool port_set_flag;
extern atomic<int> port_num;
extern  bool operator==(const datapack_fd& lhs, const datapack_fd& rhs);
bool run_gui{true};


graphics::graphics(int win_h, int win_w){
    if (!glfwInit()){
        throw std::runtime_error("COULD NOT INITILIZE GLFW");
    }
    else{
        //set up glfw version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        window = glfwCreateWindow(win_w, win_h, "FAN CONTROL", NULL, NULL);
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
   
    if(!port_set_flag){
        static int port_local{0};
        ImGui::Begin("setup");
        ImGui::Text("Select serial port:");
        ImGui::Combo("detected ports: ", &port_local, port_list.c_str());
        port_num = port_local;
        if(port_local != 0){
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
        ImGui::Begin("Fan control by Firemonkeyy");

    
        ImGui::SliderInt("FAN 1", &ui_fe.pwm_f1, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f1);

        ImGui::SliderInt("FAN 2", &ui_fe.pwm_f2, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f2);

        ImGui::SliderInt("FAN 3", &ui_fe.pwm_f3, 0, 100); 
        ImGui::SameLine(); 
        ImGui::Text("RPM: %d", ui_be.rpm_f3);

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),"TEMPERATURE: %.1f", ui_be.temperature);

        ImGui::InputInt("FAN Hz", &ui_fe.freq_pwm, 100, 500);
        if(ui_fe.freq_pwm>=MAX_HZ){
            ui_fe.freq_pwm = 22000;    
        }
        {
            lock_guard<mutex> lk(m_ui2core);
                if(q_ui2core.size()<3 && !(ui_fe==ui_fe_ans)){
                    ui_fe_ans = ui_fe;
                    q_ui2core.push(ui_fe);
                }
        }

        if (ImGui::Button("Close")){
            run_gui = false;
            ImGui::End();
            return true;
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
cmake_minimum_required(VERSION 3.1)

project(fan_control CXX)

add_executable(${PROJECT_NAME} src/core.cpp src/main.cpp src/render.cpp src/serial_communication.cpp)

target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/imgui_all.o)

target_link_libraries(${PROJECT_NAME} glfw GL pthread serial ${CMAKE_DL_LIBS})



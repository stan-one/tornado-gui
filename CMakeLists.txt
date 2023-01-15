cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)

project(fan_control CXX)

add_executable(${PROJECT_NAME} src/core.cpp src/main.cpp src/render.cpp src/serial_communication.cpp src/shared_data.cpp)

target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/imgui_all.o)

target_link_libraries(${PROJECT_NAME} glfw GL pthread serial ${CMAKE_DL_LIBS})

# Locate GTest
find_package(GTest REQUIRED)
include(GoogleTest)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(test_core test/test_core.cpp src/core.cpp src/serial_communication.cpp)
target_link_libraries(test_core GTest::GTest)
target_link_libraries(test_core pthread serial ${CMAKE_DL_LIBS})
cmake_minimum_required(VERSION 3.20)
project(ChatClient)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

add_executable(client src/main.cpp src/app.cpp src/frame.cpp)

target_include_directories(client PUBLIC includes/)
target_compile_options(client PUBLIC -std=c++20 -Wall)
target_link_libraries(client ${wxWidgets_LIBRARIES})

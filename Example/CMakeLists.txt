cmake_minimum_required(VERSION 3.25)
project(example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example main.cpp "NativeWindow.cpp" "NativeWindow.h")

target_link_libraries(example PUBLIC
    volk
    stb
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/drhi.lib
)
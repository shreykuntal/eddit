cmake_minimum_required(VERSION 3.20)
project(eddit)

set(CMAKE_CXX_STANDARD 17)

add_executable(eddit src/main.cpp src/server.cpp)

target_include_directories(eddit PRIVATE include)
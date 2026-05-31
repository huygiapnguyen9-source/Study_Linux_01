cmake_minimum_required(VERSION 3.10)
project(LinuxFork VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fork_app main.cpp)


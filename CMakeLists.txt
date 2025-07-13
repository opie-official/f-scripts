cmake_minimum_required(VERSION 3.29)
project(fscripts)

set(CMAKE_CXX_STANDARD 26)

add_executable(out out.cpp)
add_executable(fcat fcat.cpp)
add_executable(flc flc.cpp)


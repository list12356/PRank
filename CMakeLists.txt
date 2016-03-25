cmake_minimum_required(VERSION 3.3)
project(PRank_1_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Model.cpp Model.h)
add_executable(PRank_1_1 ${SOURCE_FILES})
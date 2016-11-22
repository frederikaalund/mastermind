cmake_minimum_required(VERSION 3.5)
project(mastermind)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES main.cpp render.cpp render.hpp info.h)
add_executable(mastermind ${SOURCE_FILES})
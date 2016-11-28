cmake_minimum_required(VERSION 3.5)
project(mastermind)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp render.cpp render.hpp code_pegs.hpp)
add_executable(mastermind ${SOURCE_FILES})

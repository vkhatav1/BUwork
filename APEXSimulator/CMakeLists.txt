cmake_minimum_required(VERSION 3.8)
project(SimulatorProject)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c SimulatorProject.h)
add_executable(SimulatorProject ${SOURCE_FILES})
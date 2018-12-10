cmake_minimum_required(VERSION 3.0)

project(incrementalptxjit)
set (CMAKE_CXX_STANDARD 11)

include_directories(include/)

add_executable(incrementalptxjit src/main.cpp src/argc_helper.cpp)

install(TARGETS incrementalptxjit RUNTIME DESTINATION bin)

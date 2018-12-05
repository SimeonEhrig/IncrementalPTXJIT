cmake_minimum_required(VERSION 3.0)

project(incrementalptxjit)

add_executable(incrementalptxjit main.cpp)

install(TARGETS incrementalptxjit RUNTIME DESTINATION bin)

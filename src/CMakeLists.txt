cmake_minimum_required(VERSION 3.8)

project(libgc_playground)

set(CMAKE_CXX_STANDARD 14)

add_library(libgc_playground STATIC GC/GC.cpp)
target_include_directories(libgc_playground PUBLIC "../${GC_PLAYGROUND_INCLUDE}")

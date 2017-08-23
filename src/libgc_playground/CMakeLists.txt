cmake_minimum_required(VERSION 3.5)

project(${LIBGC_PLAYGROUND})

set(CMAKE_CXX_STANDARD 14)

add_library(${LIBGC_PLAYGROUND} STATIC GC/Collector.cpp)
target_include_directories(${LIBGC_PLAYGROUND} PUBLIC ${GC_PLAYGROUND_INCLUDE})

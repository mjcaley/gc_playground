cmake_minimum_required(VERSION 3.8)

project(${LIBGC_PLAYGROUND})

set(CMAKE_CXX_STANDARD 14)

add_library(${LIBGC_PLAYGROUND} INTERFACE)
target_include_directories(${LIBGC_PLAYGROUND} INTERFACE ${GC_PLAYGROUND_INCLUDE})

cmake_minimum_required(VERSION 3.8)
project(${PERFTEST})

set(CMAKE_CXX_STANDARD 14)

set(PERFTEST_SOURCES
        performance.hpp
        performance.cpp
        main.cpp)

add_executable(${PERFTEST} ${PERFTEST_SOURCES})
target_include_directories(${PERFTEST} PUBLIC ${GC_PLAYGROUND_INCLUDE})

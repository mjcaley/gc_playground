cmake_minimum_required(VERSION 3.8)
project(perftest)

set(CMAKE_CXX_STANDARD 14)

set(PERFTEST_SOURCES
        performance.hpp
        performance.cpp
        main.cpp)

add_executable(perftest ${PERFTEST_SOURCES})
target_include_directories(perftest PUBLIC ${GC_PLAYGROUND_INCLUDE})
target_link_libraries(perftest libgc_playground)

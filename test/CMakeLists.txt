cmake_minimum_required(VERSION 3.8)
project(gc_playground_test)

set(CMAKE_CXX_STANDARD 14)

set(TEST_SOURCES
        test_main.cpp)

add_executable(tests ${TEST_SOURCES})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE "../${CATCH_INCLUDE}")

target_include_directories(tests PUBLIC "../${GC_PLAYGROUND_INCLUDE}")
target_link_libraries(tests libgc_playground Catch)

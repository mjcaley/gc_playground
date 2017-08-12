cmake_minimum_required(VERSION 3.8)
#project(libgc_playground)

set(GC_PLAYGROUND_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/GC")
set(CATCH_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/Catch")

# GC Playground library
add_subdirectory(src/libgc_playground)

# Demo program
add_subdirectory(src/demo)

# Performance test
add_subdirectory(src/perftest)

# Tests
add_subdirectory(test)

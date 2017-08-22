cmake_minimum_required(VERSION 3.8)

set(GC_PLAYGROUND_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/GC")
set(CATCH_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/Catch")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
enable_testing()

# GC Playground library
add_subdirectory(src/libgc_playground)

# Demo program
add_subdirectory(src/demo)

# Performance test
add_subdirectory(src/perftest)

# Tests
add_subdirectory(test)

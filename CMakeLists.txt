cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GC_PLAYGROUND_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/GC")
set(CATCH_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/Catch")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
enable_testing()

# GC Playground library
set(LIBGC_PLAYGROUND libgc_playground)
add_subdirectory(src/libgc_playground)

# Demo program
set(DEMO demo)
add_subdirectory(src/demo)

# Performance test
set(PERFTEST perftest)
add_subdirectory(src/perftest)

# Tests
set(TESTS tests)
add_subdirectory(test)

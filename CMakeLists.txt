cmake_minimum_required(VERSION 3.8)
#project(libgc_playground)

set(GC_PLAYGROUND_INCLUDE include/GC)
set(CATCH_INCLUDE include/Catch)

# GC Playground library
add_subdirectory(src)

# Demo program
add_subdirectory(demo)

# Performance test
add_subdirectory(perftest)

# Tests
add_subdirectory(test)

cmake_minimum_required(VERSION 3.8)

set(GC_PLAYGROUND_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/GC")
set(NEW_GC_PLAYGROUND_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/NewGC")
set(CATCH_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/Catch")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
enable_testing()

# GC Playground library
set(LIBGC_PLAYGROUND libgc_playground)
add_subdirectory(src/libgc_playground)

# Demo program
set(DEMO demo)
add_subdirectory(src/demo)

# New Demo program
set(NEW_DEMO new_demo)
add_subdirectory(src/new_demo)

# New Demo program
set(NEW_DEMO2 new_demo2)
add_subdirectory(src/new_demo2)

# Performance test
set(PERFTEST perftest)
add_subdirectory(src/perftest)

# Tests
set(TESTS tests)
add_subdirectory(test)

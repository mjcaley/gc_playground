cmake_minimum_required(VERSION 3.8)
project(gc_playground_test)

set(CMAKE_CXX_STANDARD 14)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE})

set(TEST_SOURCES
        test_main.cpp
        test_object.cpp
        test_valueobject.cpp)

add_executable(gc_test ${TEST_SOURCES})

target_include_directories(gc_test PUBLIC ${GC_PLAYGROUND_INCLUDE})
target_link_libraries(gc_test libgc_playground Catch)

enable_testing()
add_test(NAME TestBase COMMAND gc_test)

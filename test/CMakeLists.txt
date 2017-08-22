cmake_minimum_required(VERSION 3.8)
project(tests)

set(CMAKE_CXX_STANDARD 14)

# Options for CodeCoverage
include(CodeCoverage)
set(COVERAGE_EXCLUDES
        */test/*
        */include/Catch/*
        */Applications/*
        */v1/*)
APPEND_COVERAGE_COMPILER_FLAGS()

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE})

set(TEST_SOURCES
        test_main.cpp
        test_gc.cpp
        test_object.cpp
        test_ref.cpp
        test_utility.cpp
        test_valueobject.cpp
        test_weakref.cpp)

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests PUBLIC ${GC_PLAYGROUND_INCLUDE})
target_link_libraries(tests libgc_playground Catch)

setup_target_for_coverage(NAME tests_coverage EXECUTABLE tests DEPENDENCIES tests)

enable_testing()
add_test(NAME TestBase COMMAND tests)

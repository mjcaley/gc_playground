cmake_minimum_required(VERSION 3.5)

project(${TESTS})

set(CMAKE_CXX_STANDARD 14)

# Options for CodeCoverage
include(CodeCoverage)
set(COVERAGE_EXCLUDES
        '/Applications/*'
        '${CMAKE_SOURCE_DIR}/include/Catch/*'
        '${CMAKE_SOURCE_DIR}/test/*'
        )
APPEND_COVERAGE_COMPILER_FLAGS()

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE})

set(TEST_SOURCES
        test_main.cpp
        test_collector.cpp
        test_construct_array.cpp
        test_object.cpp
        test_ptr.cpp
        test_ref.cpp
        test_ref_traverse.cpp
        test_traverse.cpp
        test_weakref.cpp)

add_executable(${TESTS} ${TEST_SOURCES})
target_link_libraries(${TESTS} ${LIBGC_PLAYGROUND} Catch)

setup_target_for_coverage(NAME coverage
                          EXECUTABLE ${TESTS}
                          DEPENDENCIES ${TESTS} ${LIBGC_PLAYGROUND})

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS ${TESTS} coverage)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(${TESTS})

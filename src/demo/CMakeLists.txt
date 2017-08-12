project(demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo main.cpp)
target_link_libraries(demo libgc_playground)

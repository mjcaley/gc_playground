project(${NEW_DEMO})

set(CMAKE_CXX_STANDARD 17)

add_executable(${NEW_DEMO} main.cpp)
target_include_directories(${NEW_DEMO} PUBLIC ${NEW_GC_PLAYGROUND_INCLUDE})

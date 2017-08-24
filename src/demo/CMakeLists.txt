project(${DEMO})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${DEMO} main.cpp)
target_link_libraries(${DEMO} ${LIBGC_PLAYGROUND})

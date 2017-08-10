cmake_minimum_required(VERSION 3.8)
project(gc_playground)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(SOURCE_FILES
        include/GC/All.hpp
        include/GC/GC.hpp
        include/GC/Object.hpp
        include/GC/Ref.hpp
        include/GC/Utility.hpp
        include/GC/ValueObject.hpp
        include/GC/WeakRef.hpp
        src/GC/GC.cpp
        src/main.cpp
        src/performance.cpp
        src/performance.hpp)

add_executable(gc_playground ${SOURCE_FILES})
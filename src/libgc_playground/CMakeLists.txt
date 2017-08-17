cmake_minimum_required(VERSION 3.8)

project(libgc_playground)

set(CMAKE_CXX_STANDARD 14)

set(GC_PLAYGROUND_SOURCES
        ${GC_PLAYGROUND_INCLUDE}/GC/All.hpp
        ${GC_PLAYGROUND_INCLUDE}/GC/GC.hpp
        ${GC_PLAYGROUND_INCLUDE}/GC/Object.hpp
        ${GC_PLAYGROUND_INCLUDE}/GC/Ref.hpp
        ${GC_PLAYGROUND_INCLUDE}/GC/Utility.hpp
        ${GC_PLAYGROUND_INCLUDE}/GC/ValueObject.hpp
        ${GC_PLAYGROUND_INCLUDE}/GC/WeakRef.hpp
        GC/GC.cpp)

add_library(libgc_playground STATIC ${GC_PLAYGROUND_SOURCES})
target_include_directories(libgc_playground PUBLIC ${GC_PLAYGROUND_INCLUDE})

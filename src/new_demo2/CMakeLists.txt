project(${NEW_DEMO2})


add_executable(${NEW_DEMO2} main.cpp)
target_include_directories(${NEW_DEMO2} PUBLIC ${NEW_GC_PLAYGROUND_INCLUDE})
target_compile_features(${NEW_DEMO2} PUBLIC cxx_std_17)

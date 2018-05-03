project(${NEW_DEMO})


add_executable(${NEW_DEMO} main.cpp)
target_include_directories(${NEW_DEMO} PUBLIC ${NEW_GC_PLAYGROUND_INCLUDE})
target_compile_features(${NEW_DEMO} PUBLIC cxx_std_17)

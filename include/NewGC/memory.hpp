#pragma once

#include "allocation.hpp"
#include "pointer.hpp"
#include "frame.hpp"


template<typename T>
void new_pointer(std::size_t num = 1)
{
    auto a = allocate<T>(num);
    auto p = Pointer<T>(a);
    
    return p;
}

#pragma once

#include <cstdlib>


struct Allocation
{
    Allocation(void* pointer) : pointer(pointer) {};
    Allocation(const Allocation&) = delete;
    Allocation(Allocation&& a)
    {
        pointer = a.pointer;
        a.pointer = nullptr;
    }
    ~Allocation()
    {
        reset();
    }
    
    Allocation& operator=(const Allocation&) = delete;
    Allocation& operator=(Allocation&& a)
    {
        pointer = a.pointer;
        a.pointer = nullptr;

        return *this;
    }

    void* pointer;
    int mark { -1 };
    
    void reset()
    {
        std::free(pointer);
    };
};

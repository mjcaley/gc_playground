#pragma once

#include <cstdlib>


struct Allocation
{
    Allocation(std::size_t length, void* pointer) : length(length), pointer(pointer) {};
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

    std::size_t length;
    void* pointer;  // Make private?
    int mark { -1 };
    
    void reset()
    {
        std::free(pointer);
    };
};

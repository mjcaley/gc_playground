#pragma once

#include <cstdlib>
#include <new>


struct Allocation
{
    Allocation(std::size_t length, void* pointer) : length(length), pointer(pointer) {};
    Allocation(std::size_t length, void* pointer, int mark) : length(length), pointer(pointer), mark(mark) {};
    
    std::size_t length;
    void* pointer;  // Make private?
    int mark { -1 };
    
    void release();
};

template<typename T>
void* allocate(std::size_t size)
{
    void* allocated = std::calloc(sizeof(T), size);
    if (allocated == nullptr)
    {
        throw std::bad_alloc();
    }
    return allocated;
}

template<typename T>
Allocation alloc(std::size_t size)
{
    void* allocated = std::calloc(sizeof(T), size);
    if (allocated == nullptr)
    {
        throw std::bad_alloc();
    }
    return Allocation {size, allocated};
}

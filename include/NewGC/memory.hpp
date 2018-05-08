#pragma once

#include <cstdlib>
#include <new>
#include <forward_list>

#include "allocation.hpp"


struct MemoryManager
{
    std::forward_list<Allocation> allocated;
    
    template<typename T>
    Allocation allocate(std::size_t size)
    {
        void* memory = std::calloc(sizeof(T), size);
        if (memory == nullptr)
        {
            throw std::bad_alloc();
        }
        return Allocation {size, memory};
    }
};
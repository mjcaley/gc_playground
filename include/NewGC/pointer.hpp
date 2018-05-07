#pragma once

#include "collectable.hpp"

struct Allocation;
struct Frame;


struct PointerBase : public Collectable
{
    PointerBase() : allocation(nullptr) {};
    PointerBase(Allocation* a) : allocation(a) {};
    PointerBase(const PointerBase& p) : allocation(p.allocation) {};
    PointerBase(const PointerBase& p, int position) : allocation(p.allocation), position(position) {};
    
    Allocation* allocation;
    int position { 0 };
};

template<typename T>
struct Pointer : public PointerBase
{
    using type = T;
    
    Pointer() : PointerBase(nullptr) {};
    Pointer(Allocation* a) : PointerBase(a) {};
    Pointer(const Pointer& p) : PointerBase(p.allocation) {};
    Pointer(const Pointer& p, int position) : PointerBase(p.allocation, position) {};
    
    type& operator*()
    {
        return *(static_cast<type*>(allocation->pointer) + position);
    }
    
    Pointer& operator++()
    {
        if ((position + 1) > (allocation->length - 1))
        {
            throw std::out_of_range("Pointer out of bounds");
        }
        ++position;
        return *this;
    }
    
    Pointer operator++(int)
    {
        if ((position + 1) > (allocation->length - 1))
        {
            throw std::out_of_range("Pointer out of bounds");
        }
        auto old_p = Position(*this);
        ++position;
        return old_p;
    }
    
    Pointer& operator--()
    {
        if ((position - 1) < 0)
        {
            throw std::out_of_range("Pointer out of bounds");
        }
        --position;
        return *this;
    }
    
    Pointer operator--(int)
    {
        if ((position - 1) > 0)
        {
            throw std::out_of_range("Pointer out of bounds");
        }
        auto old_p = Position(*this);
        --position;
        return old_p;
    }
    
    type* operator->()
    {
        return get();
    }
    
    type* get()
    {
        return static_cast<type*>(allocation->pointer);
    }
};

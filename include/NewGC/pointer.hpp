#pragma once

#include "collectable.hpp"


struct Allocation;

struct PointerBase : public Collectable
{
    PointerBase() : allocation(nullptr) {};
    PointerBase(Allocation* a) : allocation(a) {};
    PointerBase(const PointerBase& p) : allocation(p.allocation) {};
    
    Allocation* allocation;

    virtual void mark(int new_mark) override
    {
        allocation->mark = new_mark;
    }
};

template<typename T>
struct Pointer : public PointerBase
{
    using type = T;
    
    Pointer() : PointerBase(nullptr) {};
    Pointer(Allocation* a) : PointerBase(a) {};
    Pointer(const Pointer& p) : PointerBase(p.allocation) {};
    Pointer(const PointerBase& p) : PointerBase(p) {};

    PointerBase to_base_pointer()
    {
        return PointerBase(allocation);
    }
    
    type& operator*()
    {
        return *(static_cast<type*>(allocation->pointer));
    }
    
    // Pointer& operator++()
    // {
    //     if ((position + 1) > (allocation->length - 1))
    //     {
    //         throw std::out_of_range("Pointer out of bounds");
    //     }
    //     ++position;
    //     return *this;
    // }
    
    // Pointer operator++(int)
    // {
    //     if ((position + 1) > (allocation->length - 1))
    //     {
    //         throw std::out_of_range("Pointer out of bounds");
    //     }
    //     auto old_p = Position(*this);
    //     ++position;
    //     return old_p;
    // }
    
    // Pointer& operator--()
    // {
    //     if ((position - 1) < 0)
    //     {
    //         throw std::out_of_range("Pointer out of bounds");
    //     }
    //     --position;
    //     return *this;
    // }
    
    // Pointer operator--(int)
    // {
    //     if ((position - 1) > 0)
    //     {
    //         throw std::out_of_range("Pointer out of bounds");
    //     }
    //     auto old_p = Position(*this);
    //     --position;
    //     return old_p;
    // }
    
    type* operator->()
    {
        return get();
    }
    
    type* get()
    {
        return static_cast<type*>(allocation->pointer);
    }
};

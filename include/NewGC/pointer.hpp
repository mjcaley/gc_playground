#pragma once

#include "allocation.hpp"


struct PointerBase
{
    PointerBase() : allocation(nullptr) {};
    PointerBase(Allocation* a) : allocation(a) {};
    PointerBase(const PointerBase& p) : allocation(p.allocation) {};
    
    int current_mark { -1 };
    Allocation* allocation;

    virtual void mark(int new_mark) = 0;
};

template<typename T>
struct Pointer : public PointerBase
{
    using type = T;
    
    Pointer() : PointerBase(nullptr) {};
    Pointer(Allocation* a) : PointerBase(a) {};
    Pointer(const Pointer& p) : PointerBase(p.allocation) {};
    Pointer(const PointerBase& p) : PointerBase(p) {};
    
    type& operator*()
    {
        return *(static_cast<type*>(allocation->pointer));
    }
    
    type* operator->()
    {
        return get();
    }
    
    type* get()
    {
        return static_cast<type*>(allocation->pointer);
    }

    virtual void mark(int new_mark) override
    {
        if (current_mark != new_mark)
        {
            current_mark = new_mark;
            allocation->mark = new_mark;
            get()->mark(new_mark);
        }
    }
};

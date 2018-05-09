#pragma once

#include "collectable.hpp"


struct Allocation;

struct PointerBase : public Collectable
{
    PointerBase() : allocation(nullptr) {};
    PointerBase(Allocation* a) : allocation(a) {};
    PointerBase(const PointerBase& p) : allocation(p.allocation) {};
    
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

    // PointerBase to_base_pointer()
    // {
    //     return PointerBase(allocation);
    // }
    
    void construct()
    {
        operator*() = T();
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

    virtual void mark(int new_mark) override
    {
        std::cout << "Pointer mark!" << std::endl;
        if (current_mark != new_mark)
        {
            current_mark = new_mark;
            allocation->mark = new_mark;
            // std::cout << "Allocation marked" << std::endl;
            // std::cout << "Pointer's pointer " << get() << std::endl;
            // void* ptr_copy = allocation->pointer;
            // type* ptr_cast = static_cast<type*>(ptr_copy);
            // std::cout << "Allocation ptr (casted) " << ptr_cast << std::endl;
            // std::cout << "Allocation ptr value " << ptr_cast->value << std::endl;
            // std::cout << "Allocation ptr mark " << ptr_cast->current_mark << std::endl;
            // type* casted_var = static_cast<type*>(allocation->pointer);
            // casted_var->mark(new_mark);
            // operator*().mark(new_mark);
            // static_cast<type*>(allocation->pointer)->mark(new_mark);
        }
    }
};

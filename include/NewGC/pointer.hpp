#pragma once

#include <cstddef>

#include "allocation.hpp"


struct PointerBase
{
    PointerBase() = default;

    virtual void mark(int new_mark) = 0;
};


template<typename T>
struct PointerLiteral : PointerBase
{
    using type = T;
    
    PointerLiteral() = default;
    PointerLiteral(T literal) : literal(literal) {};
    PointerLiteral(const PointerLiteral& p) : literal(p.literal) {};
    
    type& operator*()
    {
        return literal;
    }
    
    type* operator&()
    {
        return &literal;
    }
    
    void operator=(const T& other)
    {
        literal = other;
    }
    
    type get()
    {
        return literal;
    }
    
    T literal;
    
    virtual void mark(int new_mark) {};
};


struct PointerAllocation : public PointerBase
{
    PointerAllocation() : allocation(nullptr) {};
    PointerAllocation(Allocation* a) : allocation(a) {};
    PointerAllocation(const PointerAllocation& p) : allocation(p.allocation) {};
    
    int current_mark { -1 };
    Allocation* allocation;
    
    virtual void mark(int new_mark) = 0;
};

template<typename T>
struct Pointer : public PointerAllocation
{
    using type = T;
    
    Pointer() : PointerAllocation(nullptr) {};
    Pointer(Allocation* a) : PointerAllocation(a) {};
    Pointer(const Pointer& p) : PointerAllocation(p.allocation) {};
    
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


template<>
struct Pointer<int> : public PointerLiteral<int>
{
    Pointer(const Pointer& p) : PointerLiteral(p.literal) {};
    Pointer(const int literal) : PointerLiteral(literal) {};
};

template<>
struct Pointer<std::int64_t> : public PointerLiteral<std::int64_t> {};

template<>
struct Pointer<double> : public PointerLiteral<double> {};

template<>
struct Pointer<bool> : public PointerLiteral<bool> {};

template<>
struct Pointer<std::byte> : public PointerLiteral<std::byte> {};

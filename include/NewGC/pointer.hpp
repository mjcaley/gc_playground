#pragma once


struct Allocation;
struct Frame;

template<typename T>
struct Pointer
{
    using type = T;
    
    Pointer(Allocation* a) : allocation(a) {};
    Pointer(const Pointer& p) : allocation(p.allocation) {};
    Pointer(const Pointer& p, int position) : allocation(p.allocation), position(position) {};
    
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
    
    type* get()
    {
        return static_cast<type*>(allocation->pointer);
    }
    
private:
    Allocation* const allocation;
    int position { 0 };
    
    friend Frame;
};

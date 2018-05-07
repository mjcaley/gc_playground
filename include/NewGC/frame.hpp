#pragma once

#include <iostream>
#include <vector>


struct Allocation;
struct Collectable;
struct StackFrames;

struct Frame
{
    template<typename T>
    void add_local(const Pointer<T>& p)
    {
        locals.emplace_back(p.allocation);
    }
    
    template<typename T>
    Pointer<T> new_pointer()
    {
        
    }
    
    void add_to_stack(Collectable* c)
    {
        
    }
    
    std::vector<Allocation*> get_locals()
    {
        return locals;
    }
    
    friend std::ostream& operator<<(std::ostream&, const Frame&);
    
private:
    std::vector<Allocation*> locals;
    std::vector<Collectable*> stack_objects;
    std::vector<PointerBase*> pointer_objects;
    Allocation* return_value { nullptr };
    
    friend StackFrames;
};

struct StackFrames
{
    void pop()
    {
        assign_return_value(frames.back(), *(frames.end() - 2));
        frames.pop_back();
    }

    Frame& push()
    {
        return frames.emplace_back();
    }

    Frame& top()
    {
        return frames.back();
    }
    
    using iterator = std::vector<Frame>::iterator;
    using const_iterator = std::vector<Frame>::const_iterator;
    
    iterator begin() { return frames.begin(); }
    const_iterator cbegin() { return frames.cbegin(); }
    
    iterator end() { return frames.end(); }
    const_iterator cend() { return frames.end(); }

private:
    std::vector<Frame> frames;
    
    void assign_return_value(const Frame& from, Frame& to)
    {
        if (from.return_value)
        {
            to.locals.emplace_back(from.return_value);
        }
    }
};

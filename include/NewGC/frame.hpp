#pragma once

#include <vector>


struct Allocation;
struct StackFrames;

struct Frame
{
    template<typename T>
    void add_local(const Pointer<T>& p)
    {
        locals.emplace_back(p.allocation);
    }
    
    // template<typename T>
    // Pointer<T> new_pointer()
    // {
        
    // }
    
    const std::vector<Allocation*> get_locals() const
    {
        return locals;
    }
    
private:
    std::vector<Allocation*> locals;
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

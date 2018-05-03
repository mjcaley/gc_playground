#pragma once

#include <vector>


struct Allocation;

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
};

struct StackFrames
{
    void pop()
    {
        frames.pop_back();
    }

    Frame& push()
    {
        frames.emplace_back();
        return top();
    }

    Frame& top
    {
        return frames.back();
    }

private:
    std::vector<Frame> frames;
};

// Frame& push_frame()
// {
//     stack_frames.emplace_front();
//     return stack_frames.front();
// }

// void pop_frame()
// {
//     stack_frames.pop_front();
// }

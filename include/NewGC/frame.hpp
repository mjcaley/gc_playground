#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <vector>


struct Collectable;
struct StackFrames;

struct FrameBase {
private:
    std::vector<PointerBase> locals;
};

template<typename T>
struct Frame : public FrameBase
{
    void add_local(PointerBase p)
    {
        locals.emplace_back(p);
    }

    std::vector<PointerBase> get_locals()
    {
        return locals;
    }

    template<typename ReturnType, typename FunctionType, typename ... Args>
    Pointer<ReturnType> call(FunctionType func, Args... args)
    {
        PointerBase return_value = func(args...);
        locals.emplace_back(return_value);

        return Pointer<ReturnType>(return_value);
    }
    
private:
    Pointer<T> return_value;
};


static std::list<FrameBase> stack_frames;

// struct StackFrames
// {
//     void pop()
//     {
        
//     }

//     Frame& push()
//     {
//         return frames.emplace_back();
//     }

//     Frame& top()
//     {
//         return frames.back();
//     }
    
//     // using iterator = std::vector<Frame>::iterator;
//     // using const_iterator = std::vector<Frame>::const_iterator;
    
//     // iterator begin() { return frames.begin(); }
//     // const_iterator cbegin() { return frames.cbegin(); }
    
//     // iterator end() { return frames.end(); }
//     // const_iterator cend() { return frames.end(); }

// private:
//     static std::unique_ptr<FrameBase> head { Frame<int>() };
//     static std::unique_ptr<FrameBase> tail { nullptr };
//     std::unique_ptr<FrameBase> prev { nullptr };
//     std::unique_ptr<FrameBase> next { nullptr };
//     // std::vector<Frame> frames;
    
//     // void assign_return_value(const Frame& from, Frame& to)
//     // {
//     //     if (from.return_value)
//     //     {
//     //         to.locals.emplace_back(from.return_value);
//     //     }
//     // }
// };

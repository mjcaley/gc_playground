#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "pointer.hpp"
#include "types.hpp"
#include "memory.hpp"


struct Frame
{
    Frame(MemoryManager& memory) : memory(memory) {};
    
    void add_local(PointerBase p)
    {
        locals.emplace_back(p);
    }

    std::vector<PointerBase> get_locals()
    {
        return locals;
    }
    
    template<typename T>
    Pointer<T> new_pointer(std::size_t num = 1)
    {
        auto& a = memory.allocated.emplace_front(memory.allocate<T>(num));
        auto p = Pointer<T>(&a);
        
        return p;
    }

    template<typename ReturnType, typename FunctionType, typename ... Args>
    Pointer<ReturnType> call(Function<FunctionType> func, Args... args)
    {
        auto& next_frame = push();
        Pointer<ReturnType> return_value = func.function(next_frame, args...);
        locals.emplace_back(return_value);
        pop();
    
        return return_value;
    }
    
    Frame& push()
    {
        next = std::make_unique<Frame>(memory);
        return *next;
    }
    
    void pop()
    {
        next.reset();
    }

private:
    MemoryManager& memory;
    std::vector<PointerBase> locals;
    std::unique_ptr<Frame> next { nullptr };
};

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
    
    // void add_local(PointerBase p)
    // {
    //     locals.emplace_back(p);
    // }

    std::vector<PointerBase*> get_locals()
    {
        std::vector<PointerBase*> local_ptrs;
        for (auto& local : locals)
        {
            local_ptrs.emplace_back(local.get());
        }
        return local_ptrs;
    }
    
    template<typename T>
    Pointer<T> new_pointer(std::size_t num = 1)
    {
        auto& a = memory.allocated.emplace_front(memory.allocate<T>(num));
        auto p = Pointer<T>(&a);
        
        return p;
    }

    template<typename ReturnType, typename FunctionType, typename ... Args>
    Pointer<ReturnType> call(Pointer<ReturnType>& return_value, const Function<FunctionType> func, const Args... args)
    {
        auto& next_frame = push();
        return_value = func.function(next_frame, args...);
        std::cout << "Pointer call addr " << &return_value << std::endl;
        locals.emplace_back(std::unique_ptr<Pointer<ReturnType>>(&return_value));
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
    
    Frame* get_next()
    {
        return next.get();
    }

private:
    MemoryManager& memory;
    std::vector<std::unique_ptr<PointerBase>> locals;
    std::unique_ptr<Frame> next { nullptr };
};

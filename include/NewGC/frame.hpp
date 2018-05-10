#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

#include "pointer.hpp"
#include "types.hpp"
#include "memory.hpp"


struct Frame
{
    Frame(MemoryManager& memory) : memory(memory) {};

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
    Pointer<T>& new_pointer(std::size_t num = 1)
    {
        auto& a = memory.allocated.emplace_front(memory.allocate<T>(num));
        auto& p = locals.emplace_back(std::make_unique<Pointer<T>>(&a));
        
        auto* raw_ptr = p.get();
        return *(static_cast<Pointer<T>*>(raw_ptr)); // have to cast the raw pointer since unique_ptr can't do it
    }

    template<typename FunctionType, typename ... Args>
    typename Function<FunctionType>::return_type& call(const Function<FunctionType>& func, Pointer<Args>&... args)
    {
        using ReturnType = typename Function<FunctionType>::return_type;
        using ReturnNoRef = typename std::remove_reference<ReturnType>::type;
        
        auto& next_frame = push();
        next_frame.locals.emplace_back(std::make_unique<Pointer<Args>>(args)...);
        auto& return_value = func.function(next_frame, args...);
        auto& local = locals.emplace_back(std::make_unique<ReturnNoRef>(return_value));
        pop();
    
        return *(static_cast<ReturnNoRef*>(local.get()));
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

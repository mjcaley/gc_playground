#pragma once

#include <iostream>
#include <functional>
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

    constexpr void add_to_locals() const {}

    template<typename T> constexpr void add_to_locals(const T& arg) const {}

    template<typename T>
    void add_to_locals(Pointer<T>& ptr)
    {
        locals.emplace_back(std::make_unique<Pointer<T>>(ptr));
    }

    template<typename Arg, typename Arg2, typename ... Args>
    void add_to_locals(Arg& arg, Arg2& arg2, Args&... args)
    {
        add_to_locals(arg);
        add_to_locals(arg2, args...);
    }

    template<typename Function, typename ... Args>
    typename std::invoke_result_t<Function, Frame&, Args...> call(const Function func, Args&&... args)
    {
        using ReturnType = typename std::invoke_result_t<Function, Frame&, Args...>;
        using ReturnNoRef = typename std::remove_reference_t<ReturnType>;
        
        auto& next_frame = push();
        next_frame.add_to_locals(args...);
        if constexpr (std::is_void<ReturnType>())
        {
            std::invoke(func, next_frame, args...);
            pop();

            return;
        }
        else
        {
            auto& return_value = std::invoke(func, next_frame, args...);
            auto& local = locals.emplace_back(std::make_unique<ReturnNoRef>(return_value));
            pop();
        
            return *(static_cast<ReturnNoRef*>(local.get()));
        }
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

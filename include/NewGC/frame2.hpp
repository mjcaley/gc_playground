#pragma once

#include <iostream>
#include <functional>
#include <list>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <utility>

#include "pointer.hpp"
#include "types.hpp"
#include "memory.hpp"
#include "strategy.hpp"


struct Frame2Base
{
    Frame2Base* previous()
    {
        return prev.get();
    }
    
protected:
    std::unique_ptr<Frame2Base> prev { nullptr };
};


template<typename ... Locals>
struct Frame2 : public Frame2Base
{
//     Frame(MemoryManager& memory) : memory(memory) {};

//     std::unordered_set<std::unique_ptr<PointerBase>>& get_locals()
//     {
//         // std::unordered_set<PointerBase*> local_ptrs;
//         // for (auto& local : locals)
//         // {
//         //     local_ptrs.emplace_back(local.get());
//         // }
//         // return local_ptrs;
//         return locals;
//     }
    
//     template<typename T>
//     Pointer<T>& new_pointer(std::size_t num = 1)
//     {
//         auto& a = memory.allocated.emplace_front(memory.allocate<T>(num));
//         auto p = locals.insert(std::make_unique<Pointer<T>>(&a));
        
//         auto* raw_ptr = p.first->get();
//         return *(static_cast<Pointer<T>*>(raw_ptr)); // have to cast the raw pointer since unique_ptr can't do it
//     }

//     constexpr void add_to_locals() const {}

//     template<typename T> constexpr void add_to_locals(const T& arg) const {}

//     template<typename T>
//     void add_to_locals(Pointer<T>& ptr)
//     {
//         locals.insert(std::make_unique<Pointer<T>>(ptr));
//     }

//     template<typename Arg, typename Arg2, typename ... Args>
//     void add_to_locals(Arg& arg, Arg2& arg2, Args&... args)
//     {
//         add_to_locals(arg);
//         add_to_locals(arg2, args...);
//     }

    template<typename Function, typename ... Args>
    typename std::invoke_result_t<Function> call(Function func, Args&&... args)
    {
        // using ReturnType = typename std::invoke_result_t<Function, Frame2*, Args...>;
        using ReturnType = typename std::invoke_result_t<Function>;
        
        Frame2<Args...>& next_frame = push<Args...>();
        if constexpr (std::is_void_v<ReturnType>)
        {
            std::invoke(func, next_frame, std::forward<Args>(args)...);
            pop();

            return;
        }
        // else if constexpr (std::is_base_of_v<PointerBase, ReturnNoRef>)
        // {
        //     ReturnType return_value = std::invoke(std::forward<Function>(func), next_frame, std::forward<Args>(args)...);
        //     pop();
        
        //     return *(static_cast<ReturnNoRef*>(local.first->get()));
        // }
        else
        {
            ReturnType return_value = std::invoke(func, next_frame, std::forward<Args>(args)...);
            pop();
        
            return return_value;
        }
    }
    
    template<typename ... FrameLocals>
    Frame2<FrameLocals...>& push()
    {
        prev = std::make_unique<Frame2<FrameLocals...>>();
        return prev;
    }
    
    void pop()
    {
        prev.reset();
    }
    
//     Frame* get_next()
//     {
//         return next.get();
//     }

// private:
//     MemoryManager& memory;
//     std::unordered_set<std::unique_ptr<PointerBase>> locals;
//     std::unique_ptr<Frame> next { nullptr };

    void mark(int new_mark)
    {
        for (std::size_t i { 0 }; i < std::tuple_size<std::tuple<Locals ...>>::value; ++i)
        {
            auto* local = std::get<i>(locals);
            if (local)
            {
                local->mark(new_mark);
            }
        }
    }

    std::tuple<Locals* ...> locals;
};




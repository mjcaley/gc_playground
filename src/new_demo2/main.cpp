// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <functional>
#include <iostream>
#include <forward_list>
#include <memory>
#include <unordered_set>
#include <utility>
#include <string>
#include <vector>


#include "frame2.hpp"



// int num_allocations(MemoryManager& memory)
// {
//     int allocs { 0 };
//     for (auto& allocation : memory.allocated)
//     {
//         ++allocs;
//     }
    
//     return allocs;
// }


// Pointer<Int>& fib(Frame& frame, Pointer<Int>& num);
// std::function<Pointer<Int>&(Frame&, Pointer<Int>&)> fib_func_ptr { &fib };
// Pointer<Int>& fib(Frame& frame, Pointer<Int>& num)
// {
//     if (num->value <= 1)
//     {
//         return num;
//     }
    
//     auto& left = frame.new_pointer<Int>();
//     auto& right = frame.new_pointer<Int>();
//     left->value = num->value - 1;
//     right->value = num->value - 2;
    
//     auto& left_ret = frame.call(fib_func_ptr, left);
//     auto& right_ret = frame.call(fib_func_ptr, right);
    
//     auto& return_val = frame.new_pointer<Int>();
//     return_val->value = left_ret->value + right_ret->value;
    
//     return return_val;
// }


// Pointer<Float>& run(Frame& frame);
// std::function<Pointer<Float>&(Frame&)> run_func_ptr { &run };
// Pointer<Float>& run(Frame& frame)
// {
//     auto& num = frame.new_pointer<Float>();
//     num = Pointer<Float>(num);
//     auto& disposable = frame.new_pointer<Float>();
//     disposable = Pointer<Float>(disposable);
//     num->value = 4.2;
    
//     return num;
// }


// void mult_locals(Frame& frame, Pointer<Int>&, int, float)
// {
//     auto& p = frame.new_pointer<Int>();
//     p->value = 5;
// }

// void void_func(Frame& frame)
// {
//     std::cout << "void_func called" << std::endl;
// }

// int add(Frame&, int left, int right)
// {
//     return left + right;
// }

// int entry(Frame& frame)
// {
//     frame.push();
    
//     auto& num = frame.new_pointer<Int>();
//     num->value = 20;
//     auto& result = frame.call(fib_func_ptr, num);
    
//     std::cout << result->value << std::endl;

//     frame.call(&void_func);
//     frame.call(&mult_locals, frame.new_pointer<Int>(), 5, 4.2f);
    
//     std::cout << "Regular add: " << frame.call(&add, 4, 2) << std::endl;

//     frame.pop();
//     return 0;
// }


void mark(Frame2Base* frame, int new_mark)
{
    while (frame)
    {
        // Do stuff
        frame = frame->previous();
    }
}



// int fib(Frame2Base* frame, int num)
// {
//     auto* real_frame = static_cast<Frame2<int, int, int, int>*>(frame);
//     *std::get<0>(real_frame->locals) = num;
    
//     if (num < 2)
//     {
//         return num;
//     }
    
//     *std::get<1>(real_frame->locals) = real_frame->call(&fib, num - 1);
//     *std::get<2>(real_frame->locals) = real_frame->call(&fib, num - 2);
    
//     *std::get<3>(real_frame->locals) = *std::get<1>(real_frame->locals) + *std::get<1>(real_frame->locals);
    
//     return *std::get<3>(real_frame->locals);
// }


template<typename Function, typename ... Args>
std::invoke_result_t<Function> call(Function&& func, Args&&... args)
{
    return std::invoke(func, std::forward<Args>(args)...);
}

void do_nothing(Frame2<>& frame) {}

void run()
{
    auto f = Frame2<>();
    // f.call(&do_nothing);
    call(&do_nothing, f);
}


int main()
{
    auto f2 = Frame2<int>();
    
    std::cout << "Value of first local: " << std::get<0>(f2.locals) << std::endl;
    
    auto* integer = new int;
    *integer = 42;
    std::get<0>(f2.locals) = integer;
    
    if (!std::get<0>(f2.locals))
    {
        std::cout << "nullptr" << std::endl;
    }
    else
    {
        std::cout << "Value of first local: " << std::get<0>(f2.locals) << std::endl;
    }

	return 0;
}

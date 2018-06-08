// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <forward_list>
#include <memory>
#include <unordered_set>
#include <string>
#include <vector>

#include "allocation.hpp"
#include "pointer.hpp"
#include "frame.hpp"
#include "memory.hpp"
#include "types.hpp"
#include "gc.hpp"

#include "frame.hpp"



int num_allocations(MemoryManager& memory)
{
    int allocs { 0 };
    for (auto& allocation : memory.allocated)
    {
        ++allocs;
    }
    
    return allocs;
}


Pointer<Int>& fib(Frame& frame, Pointer<Int>& num);
std::function<Pointer<Int>&(Frame&, Pointer<Int>&)> fib_func_ptr { &fib };
Pointer<Int>& fib(Frame& frame, Pointer<Int>& num)
{
    if (num->value <= 1)
    {
        return num;
    }
    
    auto& left = frame.new_pointer<Int>();
    auto& right = frame.new_pointer<Int>();
    left->value = num->value - 1;
    right->value = num->value - 2;
    
    auto& left_ret = frame.call(fib_func_ptr, left);
    auto& right_ret = frame.call(fib_func_ptr, right);
    
    auto& return_val = frame.new_pointer<Int>();
    return_val->value = left_ret->value + right_ret->value;
    
    return return_val;
}


Pointer<Float>& run(Frame& frame);
std::function<Pointer<Float>&(Frame&)> run_func_ptr { &run };
Pointer<Float>& run(Frame& frame)
{
    auto& num = frame.new_pointer<Float>();
    num = Pointer<Float>(num);
    auto& disposable = frame.new_pointer<Float>();
    disposable = Pointer<Float>(disposable);
    num->value = 4.2;
    
    return num;
}


void mult_locals(Frame& frame, Pointer<Int>&, int, float)
{
    auto& p = frame.new_pointer<Int>();
    p->value = 5;
}

void void_func(Frame& frame)
{
    std::cout << "void_func called" << std::endl;
}

int add(Frame&, int left, int right)
{
    return left + right;
}

int entry(Frame& frame)
{
    frame.push();
    
    auto& num = frame.new_pointer<Int>();
    num->value = 20;
    auto& result = frame.call(fib_func_ptr, num);
    
    std::cout << result->value << std::endl;

    frame.call(&void_func);
    frame.call(&mult_locals, frame.new_pointer<Int>(), 5, 4.2f);
    
    std::cout << "Regular add: " << frame.call(&add, 4, 2) << std::endl;

    frame.pop();
    return 0;
}

int main()
{
    auto memory = MemoryManager();
    auto frame = Frame(memory);
    
    auto& ret = frame.call(run_func_ptr);
    std::cout << ret->value << std::endl;
    
    
    entry(frame);
    
    std::cout << "Allocations:" << num_allocations(memory) << std::endl;
    // for (auto& allocation : memory.allocated)
    // {
    //     std::cout << "Allocation " << allocation.pointer << std::endl;
    // }
    
    mark(frame);
    sweep(memory.allocated);

    std::cout << "Allocations:" << num_allocations(memory) << std::endl;
    // for (auto& allocation : memory.allocated)
    // {
    //     std::cout << "Allocation " << allocation.pointer << std::endl;
    // }


    // std::unordered_set<std::unique_ptr<PointerBase>> pointers;
    // pointers.insert(std::make_unique<Pointer<Int>>());


    auto f2 = Frame2<int> {};
    

	return 0;
}

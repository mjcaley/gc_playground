// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <forward_list>
#include <memory>
#include <string>
#include <vector>

#include "allocation.hpp"
#include "pointer.hpp"
#include "frame.hpp"
#include "memory.hpp"
#include "types.hpp"



std::vector<PointerBase*> get_roots(Frame* frame)
{
    std::vector<PointerBase*> roots;
    while (frame)
    {
        auto locals = frame->get_locals();
        roots.insert(std::end(roots),
                     std::begin(locals),
                     std::end(locals));
        frame = frame->get_next();
    }
    
    return roots;
}

int current = 0;

void mark(Frame& frame)
{
    ++current;
    auto roots = get_roots(&frame);
    for (auto& root : roots)
    {
        root->mark(current);
    }
}

template<typename T>
void sweep(T& allocations)
{
    allocations.remove_if(
        [](const Allocation& a) { return a.mark != current; }
    );
}


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


int entry(Frame& frame)
{
    frame.push();
    
    auto& num = frame.new_pointer<Int>();
    num->value = 20;
    auto& result = frame.call(fib_func_ptr, num);
    
    std::cout << result->value << std::endl;

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

	return 0;
}

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


Pointer<std::int64_t>& fib(Frame& frame, Pointer<std::int64_t>& num);
std::function<Pointer<std::int64_t>&(Frame&, Pointer<std::int64_t>&)> fib_func_ptr { &fib };
Pointer<std::int64_t>& fib(Frame& frame, Pointer<std::int64_t>& num)
{
    if (*num <= 1)
    {
        return num;
    }
    
    auto& left = frame.new_pointer<std::int64_t>();
    auto& right = frame.new_pointer<std::int64_t>();
    *left = *num - 1;
    *right = *num - 2;
    
    auto& left_ret = frame.call(fib_func_ptr, left);
    auto& right_ret = frame.call(fib_func_ptr, right);
    
    auto& return_val = frame.new_pointer<std::int64_t>();
    *return_val = *left_ret + *right_ret;
    
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
    
    auto& num = frame.new_pointer<std::int64_t>();
    *num = 20;
    auto& result = frame.call(fib_func_ptr, num);
    
    std::cout << *result << std::endl;

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

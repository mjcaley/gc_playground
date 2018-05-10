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
    

    return 0;
}

int main()
{
    auto memory = MemoryManager();
    auto frame = Frame(memory);
    
    auto func = Function<Pointer<Float>&(Frame&)>{ "run", &run };
    auto& ret = frame.call(func);
    std::cout << ret->value << std::endl;
    
    std::cout << "Allocations:" << std::endl;
    for (auto& allocation : memory.allocated)
    {
        std::cout << "Allocation " << allocation.pointer << std::endl;
    }
    
    mark(frame);
    sweep(memory.allocated);

    std::cout << "Allocations:" << std::endl;
    for (auto& allocation : memory.allocated)
    {
        std::cout << "Allocation " << allocation.pointer << std::endl;
    }

	return 0;
}

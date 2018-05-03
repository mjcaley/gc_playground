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


using Byte = std::byte;
using Int = std::int64_t;
using UInt = std::uint64_t;
using Float = double;



std::forward_list<Frame> stack_frames {};


template<typename Iterator>
std::vector<Allocation*> get_roots(Iterator begin, Iterator end)
{
    std::vector<Allocation*> roots;
    for (; begin != end; ++begin)
    {
        std::cout << "get_roots frame" << std::endl;
        roots.insert(std::end(roots), std::begin(begin->get_locals()), std::end(begin->get_locals()));
    }
    std::cout << "Root size " << roots.size() << std::endl;
    return roots;
}

int current = 0;

void mark(std::forward_list<Allocation>& allocations, const std::forward_list<Frame>& stack_frames)
{
    ++current;
    auto roots = get_roots(std::begin(stack_frames), std::end(stack_frames));
    std::cout << "Size of roots " << roots.size() << std::endl;
    for (auto& root : roots)
    {
        std::cout << root->mark << std::endl;
        root->mark = current;
    }
}

void sweep(std::forward_list<Allocation>& allocations)
{
    std::remove_if(std::begin(allocations),
                   std::end(allocations),
                   [](const Allocation& a) { return a.mark != current; }
   );
}


struct Demo
{
    int first;
    double second;
    std::string third;
    Pointer<int> forth;
};

std::ostream& operator<<(std::ostream& os, const Demo& d)
{
    os << "Demo {" << d.first << ", " << d.second << ", " << d.third << "}";
    return os;
};


std::ostream& operator<<(std::ostream& os, const Allocation& a)
{
    os << "Allocation { mark: " << a.mark << ", length: " << a.length << ", addr: " << a.pointer << "}";
    return os;
}


template<typename Iterator>
void print_allocations(Iterator begin, Iterator end)
{
    for (; begin != end; ++begin)
    {
        std::cout << *begin << "\n";
    }
}


int main()
{
    
	std::forward_list<Allocation> allocated;
	auto stack = StackFrames();
	
	auto& frame = stack.push();
	
	auto a = allocate<int>(1);
	allocated.push_front(std::move(a));
	auto a2 = allocate<int>(1);
	allocated.push_front(std::move(a2));
	
	auto p2 = Pointer<int>(&(allocated.front()));
	frame.add_local(p2);
	std::cout << "Number of frames " << frame.get_locals().size() << std::endl;
	
	std::cout << "Mark before " << allocated.front().mark << std::endl;
	
	print_allocations(std::begin(allocated), std::end(allocated));
	
	mark(allocated, stack_frames);
	sweep(allocated);
	
	print_allocations(std::begin(allocated), std::end(allocated));
	
	std::cout << "Mark after " << allocated.front().mark << std::endl;
	
	stack.pop();
	
	return 0;
}

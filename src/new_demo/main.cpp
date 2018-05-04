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



template<typename Iterator>
std::vector<Allocation*> get_roots(Iterator begin, Iterator end)
{
    std::vector<Allocation*> roots;
    for (; begin != end; ++begin)
    {
        std::cout << "get_roots frame" << std::endl;
        auto locals = begin->get_locals();
        roots.insert(std::end(roots),
                     std::begin(locals),
                     std::end(locals));
    }
    std::cout << "Root size " << roots.size() << std::endl;
    return roots;
}

int current = 0;

template<typename Iterator>
void mark(Iterator stack_frames_begin, Iterator stack_frames_end)
{
    ++current;
    auto roots = get_roots(stack_frames_begin, stack_frames_end);
    std::cout << "Size of roots " << roots.size() << std::endl;
    for (auto& root : roots)
    {
        std::cout << root->mark << std::endl;
        root->mark = current;
    }
}

template<typename T>
void sweep(T& allocations)
{
    allocations.remove_if(
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

template<typename Iterator>
std::ostream& print_vector(std::ostream& os, Iterator begin, Iterator end)
{
    for (; begin != end; ++begin)
    {
        os << *begin << " ";
    }
    return os;
}

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

// std::ostream& operator<<(std::ostream& os, const Frame& frame)
// {
//     os << "Frame { ";
//     print_vector(os, std::begin(frame.get_locals()), std::end(frame.get_locals()));
//     os << "}\n";
//     return os;
// }

std::ostream& operator<<(std::ostream& os, Frame& frame)
{
    os << "Frame { ";
    print_vector(os, std::begin(frame.get_locals()), std::end(frame.get_locals()));
    os << "}\n";
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

template<typename Iterator>
void print_stack_frame_locals(Iterator begin, Iterator end)
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
	
	
// 	std::cout << "Number of frames " << frame.get_locals().size() << std::endl;
    std::cout << "locals: ";
    auto locals = stack.top().get_locals();
    print_vector(std::cout, locals.begin(), locals.end());
    std::cout << "\n";
// 	std::cout << "Stack" << stack << std::endl;
	
	std::cout << "Mark before " << allocated.front().mark << std::endl;
	
	print_allocations(std::begin(allocated), std::end(allocated));
	
	mark(std::begin(stack), std::end(stack));
	sweep(allocated);
	
	print_allocations(std::begin(allocated), std::end(allocated));
	
	std::cout << "Mark after " << allocated.front().mark << std::endl;
	
	stack.pop();
	
	return 0;
}

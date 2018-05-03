// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <forward_list>
#include <memory>
#include <string>
#include <vector>

#include "allocation.hpp"
#include "pointer.hpp"


using Byte = std::byte;
using Int = std::int64_t;
using UInt = std::uint64_t;
using Float = double;


struct Frame
{
    template<typename T>
    void add_local(const Pointer<T>& p)
    {
        locals.emplace_back(p.allocation);
    }
    
    // template<typename T>
    // Pointer<T> new_pointer()
    // {
        
    // }
    
    const std::vector<Allocation*> get_locals() const
    {
        return locals;
    }
    
private:
    std::vector<Allocation*> locals;
};

std::forward_list<Frame> stack_frames {};

Frame& push_frame()
{
    stack_frames.emplace_front();
    return stack_frames.front();
}

void pop_frame()
{
    stack_frames.pop_front();
}


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


int main()
{
    
	std::forward_list<Allocation> allocated;
	
	
	auto& frame = push_frame();
	
	auto* ptr = allocate<int>(1);
	allocated.emplace_front(1, ptr);
	auto* ptr2 = allocate<int>(1);
	allocated.emplace_front(1, ptr2);
	
	auto& a2 = allocated.front();
// 	a2.mark = 42;
	auto* a2_ptr = &a2;
	
	
	
	auto p2 = Pointer<int>(&a2);
	frame.add_local(p2);
	std::cout << frame.get_locals().size() << std::endl;
	
	std::cout << "Mark before " << a2_ptr->mark << std::endl;
	mark(allocated, stack_frames);
	sweep(allocated);
	std::cout << "Mark after " << a2_ptr->mark << std::endl;
	
	pop_frame();
	
	return 0;
}

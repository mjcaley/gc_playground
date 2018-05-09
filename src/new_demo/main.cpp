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
        std::cout << "get_roots frame" << std::endl;
        auto locals = frame->get_locals();
        roots.insert(std::end(roots),
                     std::begin(locals),
                     std::end(locals));
        frame = frame->get_next();
    }
    std::cout << "Root size " << roots.size() << std::endl;
    return roots;
}

int current = 0;

void mark(Frame& frame)
{
    ++current;
    auto roots = get_roots(&frame);
    std::cout << "Size of roots " << roots.size() << std::endl;
    for (auto& root : roots)
    {
        std::cout << "root's old mark: " << root->current_mark << std::endl;
        std::cout << "root addr " << root << std::endl;
        root->mark(current);
        std::cout << "root's new mark: " << root->current_mark << std::endl;
    }
}

template<typename T>
void sweep(T& allocations)
{
    allocations.remove_if(
        [](const Allocation& a) { return a.mark != current; }
    );
}


// struct Demo
// {
//     int first;
//     double second;
//     std::string third;
//     Pointer<int> forth;
// };

// // template<typename Iterator>
// // std::ostream& print_vector(std::ostream& os, Iterator begin, Iterator end)
// // {
// //     for (; begin != end; ++begin)
// //     {
// //         os << *begin << " ";
// //     }
// //     return os;
// // }

// // std::ostream& operator<<(std::ostream& os, const Demo& d)
// // {
// //     os << "Demo {" << d.first << ", " << d.second << ", " << d.third << "}";
// //     return os;
// // };


// // std::ostream& operator<<(std::ostream& os, const Allocation& a)
// // {
// //     os << "Allocation { mark: " << a.mark << ", length: " << a.length << ", addr: " << a.pointer << "}";
// //     return os;
// // }

// // std::ostream& operator<<(std::ostream& os, const Frame& frame)
// // {
// //     os << "Frame { ";
// //     print_vector(os, std::begin(frame.get_locals()), std::end(frame.get_locals()));
// //     os << "}\n";
// //     return os;
// // }

// // std::ostream& operator<<(std::ostream& os, Frame& frame)
// // {
// //     os << "Frame { ";
// //     print_vector(os, std::begin(frame.get_locals()), std::end(frame.get_locals()));
// //     os << "}\n";
// //     return os;
// // }



// template<typename Iterator>
// void print_allocations(Iterator begin, Iterator end)
// {
//     for (; begin != end; ++begin)
//     {
//         std::cout << *begin << "\n";
//     }
// }

// template<typename Iterator>
// void print_stack_frame_locals(Iterator begin, Iterator end)
// {
//     for (; begin != end; ++begin)
//     {
//         std::cout << *begin << "\n";
//     }
// }





// std::forward_list<Allocation> allocated {};

// template<typename T>
// Pointer<T> new_pointer()
// {
//     allocated.push_front(allocate<Float>(1));
//     auto& a = allocated.front();
//     return Pointer<T>(&a);
// }



Pointer<Float> run(Frame& frame)
{
    auto num = frame.new_pointer<Float>();
    auto disposable = frame.new_pointer<Float>();
    num->value = 4.2;
    return num;
}

int main()
{
    
// 	std::forward_list<Allocation> allocated;
// 	auto stack = StackFrames();
	
// 	auto& frame = stack.push();
	
// 	auto a = allocate<int>(1);
// 	allocated.push_front(std::move(a));
// 	auto a2 = allocate<int>(1);
// 	allocated.push_front(std::move(a2));
	
// 	auto p2 = Pointer<int>(&(allocated.front()));
// 	frame.add_local(&p2);
	
	
// // 	std::cout << "Number of frames " << frame.get_locals().size() << std::endl;
//     std::cout << "locals: ";
//     auto locals = stack.top().get_locals();
//     // print_vector(std::cout, locals.begin(), locals.end());
//     std::cout << "\n";
// // 	std::cout << "Stack" << stack << std::endl;
	
// 	std::cout << "Mark before " << allocated.front().mark << std::endl;
	
// 	// print_allocations(std::begin(allocated), std::end(allocated));
	
// 	mark(std::begin(stack), std::end(stack));
// 	sweep(allocated);
	
// 	// print_allocations(std::begin(allocated), std::end(allocated));
	
// 	std::cout << "Mark after " << allocated.front().mark << std::endl;
	
// 	stack.pop();
	
	
	
	
// 	// Stack types
// 	std::cout << '\n';
// 	auto f_mem = allocate<Float>(1);
// 	auto f = Pointer<Float>(&f_mem);
// 	f->value = 4.2;
// 	std::cout << "Float value: " << f->value << std::endl;
// 	auto e = Example();
// 	e.number = 42;
// 	e.other_num = f;
// 	std::cout << "Example struct: { number: " << e.number << ", other_num: " << e.other_num->value << " }" << std::endl;
// 	e.mark(100);
//     std::cout << "new mark: " << e.current_mark << std::endl;
    
    
//     auto array1 = Array<Float, 1>();
//     auto array2 = Array<Pointer<Float>, 1>();
	
	auto memory = MemoryManager();
	auto root_frame = Frame(memory);
	
    Function<Pointer<Float>(Frame&)> f;
    f.function = &run;
    auto p = f.function(root_frame);
    std::cout << p->value << std::endl;
    
    
    auto frame_call_return = root_frame.new_pointer<Float>();
    std::cout << "Pointer stack addr " << &frame_call_return << std::endl;
    root_frame.call(frame_call_return, f);
    std::cout << frame_call_return->value << std::endl;
    std::cout << "ending everything, hopefully that frame is destroyed" << std::endl;
    std::cout << "Pointer's pointer (on stack) " << frame_call_return.get() << " " << frame_call_return.get()->value << std::endl;
    
    mark(root_frame);
    sweep(memory.allocated);
    
    mark(root_frame);
    sweep(memory.allocated);

	return 0;
}

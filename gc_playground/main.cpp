#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "gc1.hpp"
#include "gc2.hpp"



void sub_func(GC& gc)
{
    // Allocate object and let it die
    
    auto i42 = gc.make_int(42);
}

void test_gc1()
{
        auto gc = GC();
        auto thirty_two = gc.make_int(32);
        sub_func(gc);
        auto eight = gc.make_int(8);
}


void sub_func(GC2& gc)
{
    
}

void test_gc2()
{
//        auto integer = make_integer(42);
//        for (int i {0}; i < 10; ++i)
//        {
//            used_list.pop_front();
//            integer = make_integer(i);
//            std::cout << "Value of integer: " << *integer << std::endl;
//        }
//        used_list.pop_front();
    
    auto gc = GC2();
    auto integer = gc.create(42);
    for (int i {0}; i < 10; ++i)
    {
        auto floating_point = gc.create(static_cast<float>(i));
        std::cout << "Value of integer: " << *integer << std::endl;
    }
}



int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

//    Original GC
//    test_gc1();
    
    // Second GC
    test_gc2();
    
    return 0;
}

#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "gc1.hpp"
#include "gc2.hpp"


struct Test
{
    Test(GCRef2<int> one, GCRef2<float> two) : one(one), two(two) {}
    
    GCRef2<int> one;
    GCRef2<float> two;
};

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


GCRef2<Test> sub_func(GC2& gc)
{
    auto doubles = std::vector<GCRef2<double>> { gc.create<double>(1.0),
        gc.create<double>(2.0),
        gc.create<double>(3.0),
        gc.create<double>(4.0),
        gc.create<double>(5.0) };
    auto one = gc.create(42);
    std::cout << "one " << one << std::endl;
    auto two = gc.create(4.2f);
    std::cout << "two " << two << std::endl;
    auto test = gc.create(Test(one, two));
    
    return test;
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
//        std::cout << "Value of integer: " << *integer << std::endl;
    }
    auto doubles = std::vector<GCRef2<double>> { gc.create<double>(1.0),
        gc.create<double>(2.0),
        gc.create<double>(3.0),
        gc.create<double>(4.0),
        gc.create<double>(5.0) };
    auto test = sub_func(gc);
    gc.collect();
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

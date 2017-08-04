#include <array>
#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "gc1.hpp"
#include "gc2.hpp"
#include "gc3.hpp"


struct Test
{
    Test(GCRef2<int> one, GCRef2<float> two) : one(one), two(two) {}
    
    GCRef2<int> one;
    GCRef2<float> two;
};

void sub_func(GC1& gc)
{
    // Allocate object and let it die
    
    auto i42 = gc.make_int(42);
}

void test_gc1()
{
        auto gc = GC1();
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

GC3::Ref<int> allocate_and_ditch()
{
    using namespace GC3;
    
    auto test3 = Ref<Test3>(Ref<int>(1), Ref<float>(2.2));
    auto integer = Ref<int>(42);
    
    return integer;
}

void test_gc3()
{
    using namespace GC3;
    
    auto integer = allocate_and_ditch();
    auto new_integer = Ref<int>(69);
    
//    auto int_ref = GC::create<int>(42);
//    auto int_ref2 = Ref<int>(42);
//    
//    std::vector<Ref<int>> vec { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//    auto weak_vec = to_weak_ref(vec);
//
//    std::array<Ref<int>, 10> arr { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//    auto weak_arr = to_weak_ref(arr);
//    auto strong_arr = to_ref(weak_arr);
//    
//    auto tup = std::make_tuple(Ref<int>(1), Ref<float>(2.2f), Ref<double>(3.3));
//    auto weak_tup = to_weak_ref(tup);
//    auto strong_tup = to_ref(weak_tup);
    std::cout << "I still have the integer " << *integer << std::endl;
}


template<typename T>
void f(T value) { std::cout << "generic" << std::endl; }

//template<>
template<typename U, typename std::size_t N>
void f(std::array<U, N> value) { std::cout << "array" << std::endl; }

//template<>
template<typename U>
void f(std::vector<U> value) { std::cout << "vector" << std::endl; }

//template<>
template<typename ... Types>
void f(std::tuple<Types...> value) { std::cout << "tuple" << std::endl; }




int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    f(42);
    f(std::array<int, 4> {1,2,3,4});
    f(std::vector<int>(5));
    f(std::make_tuple<int, float, double>(1, 2.2, 3.3));
    
//    Original GC
//    test_gc1();
    
    // Second GC
//    test_gc2();
    
    // Third GC
    test_gc3();
    
    return 0;
}

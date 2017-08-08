#include <array>
#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "GC/All.hpp"


using GC::Ref;
using GC::WeakRef;
using GC::Object;
using GC::ValueObject;


struct Test3
{
    Test3(Ref<int> one, Ref<float> two) : num(0), one(one), two(two) {}
    
    int num;
    WeakRef<int> one;
    WeakRef<float> two;
    
    friend void traverse(Object*, unsigned int, Test3);
};

template<>
void traverse(Test3* value, unsigned int current_mark)
{
    if (value->one)
    {
        Ref<int> one_ref = value->one;
        one_ref.mark(current_mark);
    }
    if (value->two)
    {
        Ref<float> two_ref = value->two;
        two_ref.mark(current_mark);
    }
}


struct TestB;

struct TestA
{
    WeakRef<TestB> test_b;
    
    friend void traverse(Object*, unsigned int, TestA);
};

template<>
void traverse(TestA* value, unsigned int current_mark)
{
    if (value->test_b)
    {
        Ref<TestB> ref = value->test_b;
        ref.mark(current_mark);
    }
}

struct TestB
{
    WeakRef<TestA> test_a;
    
    friend void traverse(Object*, unsigned int, TestB);
};

template<>
void traverse(TestB* value, unsigned int current_mark)
{
    if (value->test_a)
    {
        Ref<TestA> ref = value->test_a;
        ref.mark(current_mark);
    }
}



Ref<int> allocate_and_ditch()
{
    auto test3 = Ref<Test3>(Ref<int>(1), Ref<float>(2.2));
    auto integer = Ref<int>(42);
    
    return integer;
}

void cycle_test()
{
    Ref<TestA> testa;
    Ref<TestB> testb;
    testa->test_b = testb;
    testb->test_a = testa;
    GC::GC::collect();
}

void test_gc3()
{
    auto integer = allocate_and_ditch();
    auto new_integer = Ref<int>(69);
    WeakRef<int> weak_int = integer;
    Ref<int> strong_int = weak_int;
    
    auto int_ref2 = Ref<int>(42);
    int_ref2 = Ref<int>(31);
    
    std::vector<Ref<int>> vec { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<WeakRef<int>> w_vec(vec.begin(), vec.end());

    std::array<Ref<int>, 10> arr { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto weak_arr = convert(arr);
    auto strong_arr = convert(weak_arr);
    
    auto tup = std::make_tuple(Ref<int>(1), Ref<float>(2.2f), Ref<double>(3.3));
    std::tuple<WeakRef<int>, WeakRef<float>, WeakRef<double>> w_tup = tup;
    std::cout << "I still have the integer " << *integer << std::endl;
    
    cycle_test();
    GC::GC::collect();
}


int main(int argc, const char * argv[]) {
    std::cout << "GC Playground" <<std::endl;
    
    test_gc3();
    
    return 0;
}

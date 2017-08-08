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
void traverse(Object* object, unsigned int current_mark, Test3)
{
    auto* test_obj = dynamic_cast<ValueObject<Test3>*>(object);
    
    auto one_ref = test_obj->ptr->one.get_reference();
    auto two_ref = test_obj->ptr->two.get_reference();
    
    one_ref.get_pointer()->mark(current_mark);
    two_ref.get_pointer()->mark(current_mark);
}


struct TestB;

struct TestA
{
    WeakRef<TestB> test_b;
    
    friend void traverse(Object*, unsigned int, TestA);
};

template<>
void traverse(Object* object, unsigned int current_mark, TestA)
{
    auto* testa_obj = dynamic_cast<ValueObject<TestA>*>(object);

    WeakRef<TestB> weak_ref = testa_obj->ptr->test_b;
    if (weak_ref)
    {
        auto ref = weak_ref.get_reference();
        ref.get_pointer()->mark(current_mark);
    }
}

struct TestB
{
    WeakRef<TestA> test_a;
    
    friend void traverse(Object*, unsigned int, TestB);
};

template<>
void traverse(Object* object, unsigned int current_mark, TestB)
{
    auto* testa_obj = dynamic_cast<ValueObject<TestB>*>(object);
    
    WeakRef<TestA> weak_ref = testa_obj->ptr->test_a;
    if (weak_ref)
    {
        auto ref = weak_ref.get_reference();
        ref.get_pointer()->mark(current_mark);
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

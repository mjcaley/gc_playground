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

    friend void traverse(Test3*, unsigned int);
};

template<>
void traverse(Test3* value, unsigned int new_mark)
{
    if (value->one)
    {
        Ref<int> one_ref = value->one;
        one_ref.mark(new_mark);
    }
    if (value->two)
    {
        Ref<float> two_ref = value->two;
        two_ref.mark(new_mark);
    }
}


struct TestB;

struct TestA
{
    WeakRef<TestB> test_b;

    friend void traverse(TestA*, unsigned int);
};

template<>
void traverse(TestA* value, unsigned int new_mark)
{
    if (value->test_b)
    {
        Ref<TestB> ref = value->test_b;
        ref.mark(new_mark);
    }
}

struct TestB
{
    WeakRef<TestA> test_a;

    friend void traverse(TestB*, unsigned int);
};

template<>
void traverse(TestB* value, unsigned int new_mark)
{
    if (value->test_a)
    {
        Ref<TestA> ref = value->test_a;
        ref.mark(new_mark);
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




    auto array_ptr = GC::Ptr::create<int[], 5>(1,2,3,4,5);
    auto array_ref = GC::ArrayRef<int[], 5>(&array_ptr);

    std::cout << "array values: ";
    for (auto& i : *array_ref)
    {
        std::cout << i;
    }
    std::cout << std::endl;

    auto t_obj = GC::TypedObject<int>();
    auto ptr2 = GC::Ptr::create<int>(42);
//    auto array_ptr = GC::Ptr::create<int[]>(5);

    auto ref = GC::Ref2<int>(&ptr2);
    std::cout << "Ref2's value: " << *ref << std::endl;

    struct Nums
    {
        int first { 1 };
        int second { 2 };
    };
    auto ptr3 = GC::Ptr::create<Nums>();
    auto ref2 = GC::Ref2<Nums>(&ptr3);
    ref2->first = 42;
    ref2->second = 32;
    std::cout << "Ref2's values: " << ref2->first << " " << ref2->second << std::endl;

    return 0;
}

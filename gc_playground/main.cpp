#include <array>
#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "gc.hpp"

using namespace GC;


struct Test3
{
    Test3(Ref<int> one, Ref<float> two) : one(to_weak_ref(one)), two(to_weak_ref(two)) {}
    
    WeakRef<int> one;
    WeakRef<float> two;
    
    friend void mark(Object*, unsigned int, Test3);
};

template<>
void mark(Object* object, unsigned int current_mark, Test3)
{
    object->set_mark(current_mark);
    
    auto* test_obj = dynamic_cast<ValueObject<Test3>*>(object);
    
    auto one_ref = test_obj->ptr->one.get_reference();
    auto two_ref = test_obj->ptr->two.get_reference();
    
    one_ref.get_pointer()->mark_func(current_mark);
    two_ref.get_pointer()->mark_func(current_mark);
}





Ref<int> allocate_and_ditch()
{
    auto test3 = Ref<Test3>(Ref<int>(1), Ref<float>(2.2));
    auto integer = Ref<int>(42);
    
    return integer;
}

void test_gc3()
{
    auto integer = allocate_and_ditch();
    auto new_integer = Ref<int>(69);
    
    auto int_ref2 = Ref<int>(42);
    
    std::vector<Ref<int>> vec { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto weak_vec = to_weak_ref(vec);

    std::array<Ref<int>, 10> arr { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto weak_arr = to_weak_ref(arr);
    auto strong_arr = to_ref(weak_arr);
    
    auto tup = std::make_tuple(Ref<int>(1), Ref<float>(2.2f), Ref<double>(3.3));
    auto weak_tup = to_weak_ref(tup);
    auto strong_tup = to_ref(weak_tup);
    std::cout << "I still have the integer " << *integer << std::endl;
}


int main(int argc, const char * argv[]) {
    test_gc3();
    
    return 0;
}

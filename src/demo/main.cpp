#include <array>
#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "GC/GC.hpp"

using GC::Ref;
using GC::WeakRef;


void test_gc()
{
    auto r = Ref<int>(42);
}


struct Reffer
{
    Reffer(Ref<int> value) : value(value) {}
    Reffer(WeakRef<int> value) : value(value) {}

    WeakRef<int> value { Ref<int>(42) };

    friend void GC::traverse<Reffer>(Reffer&, unsigned int);
};

template<>
void GC::traverse<Reffer>(Reffer& object, unsigned int marker)
{
    auto value_ref = Ref<int>(object.value);
    traverse(value_ref, marker);
}

int main(int argc, const char * argv[]) {
    std::cout << "GC Playground" <<std::endl;

    test_gc();
    GC::collect();

    auto i = Ref<int>(42);
    std::cout << "Integer " << *i << std::endl;

    // auto ai = GC::Ref<int[5]>(1,2,3,4,5);
    // for (int num {0}; num < 5; ++num)
    // {
    //     std::cout << " [" << num << "] " << ai[num];
    // }
    // std::cout << std::endl;

    GC::collect();

    GC::Ref<int[2][2]> mai;
    // mai = { {1,2},{2,2} };

    struct Nums
    {
        int first { 1 };
        int second { 2 };
    };
    auto n = GC::Ref<Nums>();
    std::cout << "Num { first: " << n->first << ", second: " << n->second << " }" << std::endl;

    Ref<Reffer> reffer(Ref<int>(42));
    GC::collect();

    return 0;
}

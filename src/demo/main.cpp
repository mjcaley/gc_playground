#include <array>
#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

#include "GC/GC.hpp"

using GC::Ref;


void test_gc()
{
    auto r = Ref<int>(42);
}

int main(int argc, const char * argv[]) {
    std::cout << "GC Playground" <<std::endl;

    test_gc();
    GC::collect();

    auto i = Ref<int>(42);
    std::cout << "Integer " << *i << std::endl;

    auto ai = GC::Ref<int[5]>(1,2,3,4,5);
    for (int num {0}; num < 5; ++num)
    {
        std::cout << " [" << num << "] " << ai[num];
    }
    std::cout << std::endl;

    auto mai = GC::Ref<int[2][2]>();

    struct Nums
    {
        int first { 1 };
        int second { 2 };
    };
    auto n = GC::Ref<Nums>();
    std::cout << "Num { first: " << n->first << ", second: " << n->second << " }" << std::endl;

    return 0;
}

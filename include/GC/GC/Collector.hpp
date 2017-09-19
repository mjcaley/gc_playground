#pragma once

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>

#include "GC/Ptr.hpp"


namespace GC
{
    namespace Detail
    {
        extern unsigned int current_mark;
        extern std::forward_list<Ptr> used_list;
    }

    extern struct Allocations
    {
        std::uintmax_t existing { 0 };
        std::uintmax_t created { 0 };
        std::uintmax_t deleted { 0 };

        std::uintmax_t resolve();
    } allocations;

    extern float collection_percentage;

    Ptr* add(Ptr pointer);
    void mark(unsigned int marker);
    void sweep(unsigned int marker);
    void collect();
}

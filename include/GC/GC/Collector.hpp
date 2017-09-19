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
        static unsigned int current_mark { 0 };
        static std::forward_list<Ptr> used_list;
    }

    static struct Allocations
    {
        std::uintmax_t existing { 0 };
        std::uintmax_t created { 0 };
        std::uintmax_t deleted { 0 };

        std::uintmax_t resolve();
    } allocations;

    static float collection_percentage { 0.25f };

    Ptr* add(Ptr pointer);
    void mark(unsigned int marker);
    void sweep(unsigned int marker);
    void collect();
}

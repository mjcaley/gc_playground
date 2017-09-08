#pragma once

#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>

#include "GC/Ptr.hpp"


namespace GC
{
    namespace Detail
    {
        unsigned int current_mark;
        std::forward_list<Ptr> used_list;
    }

    struct Ptr;

    Ptr* add(Ptr pointer)
    {
        Detail::used_list.emplace_front(std::move(pointer));
        return &Detail::used_list.front();
    }

    void mark(unsigned int marker)
    {
        for(auto& ptr : Detail::used_list)
        {
            if (ptr.is_root())
            {
                ptr.mark(marker);
            }
        }
    }

    void sweep(unsigned int marker)
    {
        Detail::used_list.remove_if(
            [&marker](auto& ptr) {
                bool result = ptr.get_mark() != marker;
                return result;
            }
        );
    }

    void collect()
    {
        mark(Detail::current_mark);
        sweep(Detail::current_mark);
        ++Detail::current_mark;
    }
}

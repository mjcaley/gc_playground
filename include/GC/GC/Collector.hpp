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
        static unsigned int current_mark;
        static std::forward_list<Ptr> used_list;
    }

    static struct Allocations
    {
        std::uintmax_t existing { 0 };
        std::uintmax_t created { 0 };
        std::uintmax_t deleted { 0 };

        std::uintmax_t resolve()
        {
            existing += created;
            existing -= deleted;
            created = 0;
            deleted = 0;

            return existing;
        }
    } allocations;

    static float collection_percentage { 0.25 };

    static void collect();

    static Ptr* add(Ptr pointer)
    {
        Detail::used_list.emplace_front(std::move(pointer));
        ++allocations.created;
        float p = std::min(1.0f, std::max(collection_percentage, 0.0f));
        if (allocations.created > (allocations.existing / p))
        {
            collect();
        }

        return &Detail::used_list.front();
    }

    static void mark(unsigned int marker)
    {
        for(auto& ptr : Detail::used_list)
        {
            if (ptr.is_root())
            {
                ptr.mark(marker);
            }
        }
    }

    static void sweep(unsigned int marker)
    {
        Detail::used_list.remove_if(
            [&marker](auto& ptr) {
                bool result = ptr.get_mark() != marker;
                if (result)
                {
                    ++allocations.deleted;
                }
                return result;
            }
        );
    }

    static void collect()
    {
        mark(Detail::current_mark);
        sweep(Detail::current_mark);
        allocations.resolve();
        ++Detail::current_mark;
    }
}

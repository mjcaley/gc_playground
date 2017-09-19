#include <algorithm>
#include <forward_list>

#include "GC/Collector.hpp"
#include "GC/Ptr.hpp"


std::uintmax_t GC::Allocations::resolve()
{
    existing += created;
    existing -= deleted;
    created = 0;
    deleted = 0;

    return existing;
}

GC::Ptr* GC::add(GC::Ptr pointer)
{
    GC::Detail::used_list.emplace_front(std::move(pointer));
    ++GC::allocations.created;
    float p = std::min(1.0f, std::max(GC::collection_percentage, 0.0f));
    if (GC::allocations.created > (GC::allocations.existing / p))
    {
        collect();
    }

    return &Detail::used_list.front();
}

void GC::mark(unsigned int marker)
{
    for(auto& ptr : GC::Detail::used_list)
    {
        if (ptr.is_root())
        {
            ptr.mark(marker);
        }
    }
}

void GC::sweep(unsigned int marker)
{
    GC::Detail::used_list.remove_if(
        [&marker](auto& ptr) {
            bool result = ptr.get_mark() != marker;
            if (result)
            {
                ++GC::allocations.deleted;
            }
            return result;
        }
    );
}

void GC::collect()
{
    GC::mark(GC::Detail::current_mark);
    GC::sweep(GC::Detail::current_mark);
    GC::allocations.resolve();
    ++GC::Detail::current_mark;
}
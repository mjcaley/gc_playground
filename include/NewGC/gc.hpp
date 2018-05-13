#pragma once

#include <vector>

#include "pointer.hpp"
#include "frame.hpp"


int current { 0 };

struct PointerBase;

std::vector<PointerBase*> get_roots(Frame* frame)
{
    std::vector<PointerBase*> roots;
    while (frame)
    {
        auto locals = frame->get_locals();
        roots.insert(std::end(roots),
                     std::begin(locals),
                     std::end(locals));
        frame = frame->get_next();
    }
    
    return roots;
}

void mark(Frame& frame)
{
    ++current;
    auto roots = get_roots(&frame);
    for (auto& root : roots)
    {
        root->mark(current);
    }
}

template<typename T>
void sweep(T& allocations)
{
    allocations.remove_if(
        [](const Allocation& a) { return a.mark != current; }
    );
}

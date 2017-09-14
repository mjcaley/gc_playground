#pragma once

#include <array>

// #include "GC/RefFwd.hpp"


namespace GC
{
    // template<typename T, typename Enable> struct Ref;
    template<typename T> struct WeakRef;

    template<typename T> void traverse_container(T& object, unsigned int marker);
    
    template<typename T>
    void traverse(T& object, unsigned int marker) {}

    template<typename T>
    void traverse(std::enable_if_t<std::is_array<T>::value, construct_array_t<T>>& object, unsigned int marker)
    {
        traverse_container(object, marker);
    }

    template<typename T, std::size_t S>
    void traverse(std::array<T, S>& object, unsigned int marker)
    {
        traverse_container(object, marker);
    }

    template<typename T>
    void traverse(std::vector<T>& object, unsigned int marker)
    {
        traverse_container(object, marker);
    }


    template<typename T>
    void traverse_container(T& object, unsigned int marker)
    {
        for (auto& o : object)
        {
            traverse(o, marker);
        }
    }
}

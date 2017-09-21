#pragma once

#include <array>
#include <type_traits>

#include "GC/construct_array.hpp"


namespace GC
{
    template<typename T> void traverse_container(T& object, unsigned int marker);
    template<typename Iterator> void traverse_iterator(Iterator begin, Iterator end, unsigned int marker);
    
    template<typename T>
    void traverse(T& object, unsigned int marker) {}

    template<typename T>
    void traverse(std::enable_if_t<std::is_array<T>::value, construct_array_t<T>>& object, unsigned int marker)
    {
        traverse_iterator(std::begin(object), std::end(object), marker);
    }

    template<typename T, std::size_t S>
    void traverse(std::array<T, S>& object, unsigned int marker)
    {
        traverse_iterator(std::begin(object), std::end(object), marker);
    }

    template<typename T>
    void traverse(std::vector<T>& object, unsigned int marker)
    {
        traverse_iterator(std::begin(object), std::end(object), marker);
    }

    template<typename T>
    void traverse_container(T& object, unsigned int marker)
    {
        for (auto& o : object)
        {
            traverse(o, marker);
        }
    }

    template<typename Iterator>
    void traverse_iterator(Iterator begin, Iterator end, unsigned int marker)
    {
        for (; begin != end; ++begin)
        {
            traverse(*begin, marker);
        }
    }
}

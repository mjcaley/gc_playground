#pragma once

#include <array>
#include <utility>

#include "GC/Ref.hpp"
#include "GC/WeakRef.hpp"

namespace GC
{
    // Ref<->WeakRef conversion functions
    template<typename From, typename To, std::size_t ... Is>
    To convert_impl(From& from, std::index_sequence<Is...>)
    {
        return To { std::get<Is>(from)... };
    }
    
    template<typename T, std::size_t N>
    std::array<WeakRef<T>, N> convert(std::array<Ref<T>, N>& from)
    {
        return convert_impl<std::array<Ref<T>, N>, std::array<WeakRef<T>, N>>(from, std::make_index_sequence<N>());
    }
    
    template<typename T, std::size_t N>
    std::array<Ref<T>, N> convert(std::array<WeakRef<T>, N>& from)
    {
        return convert_impl<std::array<WeakRef<T>, N>, std::array<Ref<T>, N>>(from, std::make_index_sequence<N>());
    }
}

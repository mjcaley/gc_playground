#pragma once

#include <array>
#include <utility>


namespace GC
{
//    template<typename T> struct Ref;
//    template<typename T> struct WeakRef;
//
//    // Ref<->WeakRef conversion functions
//    template<typename To, typename From, std::size_t ... Is>
//    To convert_impl(From& from, std::index_sequence<Is...>)
//    {
//        return To { std::get<Is>(from)... };
//    }
//
//    template<typename T, std::size_t N>
//    std::array<WeakRef<T>, N> convert(std::array<Ref<T>, N>& from)
//    {
//        return convert_impl<std::array<WeakRef<T>, N>>(from, std::make_index_sequence<N>());
//    }
//
//    template<typename T, std::size_t N>
//    std::array<Ref<T>, N> convert(std::array<WeakRef<T>, N>& from)
//    {
//        return convert_impl<std::array<Ref<T>, N>>(from, std::make_index_sequence<N>());
//    }
}

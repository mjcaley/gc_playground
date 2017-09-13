#pragma once

#include <array>
#include <type_traits>


template<typename T, std::size_t Rank>
struct construct_array
{
    using type = typename std::array<typename construct_array<std::remove_extent_t<T>, Rank - 1>::type, std::extent<T>::value>;
};

template<typename T>
struct construct_array<T, 1>
{
    using type = typename std::array<std::remove_extent_t<T>, std::extent<T>::value>;
};

template<typename T>
using construct_array_t = typename construct_array<T, std::rank<T>::value>::type;
#include "catch.hpp"

#include <array>
#include <type_traits>

#include "GC/construct_array.hpp"


TEST_CASE( "Single dimension array to std::array", "[gc][construct_array]" )
{
    using arr = construct_array<int[10], std::rank<int[10]>::value >::type;
    bool result = typeid(arr) == typeid(std::array<int, 10>);

    REQUIRE(result);
}

TEST_CASE( "Multi-dimensional array to nested std::array", "[gc][construct_array]" )
{
    using arr = construct_array<int[10][10][10], std::rank<int[10][10][10]>::value >::type;

    REQUIRE(typeid(arr) == typeid(std::array<std::array<std::array<int, 10>, 10>, 10>));
}

TEST_CASE( "Non-array type to T", "[gc][construct_array]" )
{
    using arr = construct_array<int, std::rank<int>::value >::type;

    REQUIRE(typeid(arr) == typeid(int));
}

TEST_CASE( "construct_array helper type", "[gc][construct_array]" )
{
    using arr = construct_array_t<int[10]>;

    REQUIRE(typeid(arr) == typeid(std::array<int, 10>));
}

#include "catch.hpp"

#include "GC/GC.hpp"

#include <array>
#include <vector>


struct TraverseMock
{
    unsigned int mark { 0 };
};

namespace GC
{
    template<>
    void traverse(TraverseMock& object, unsigned int marker)
    {
        object.mark = marker;
    }
}

TEST_CASE( "Traverse passes mark", "[gc][traverse]" )
{
    TraverseMock obj;
    GC::traverse(obj, 42);

    REQUIRE(obj.mark == 42);
}

TEST_CASE( "Traverse iterator of Mocks", "[gc][traverse]" )
{
    std::vector<TraverseMock> mocks(10);

    GC::traverse_iterator(std::begin(mocks), std::end(mocks), 42);

    for (auto& mock : mocks)
    {
        REQUIRE(mock.mark == 42);
    }
}

TEST_CASE( "Traverse vector container of Mocks", "[gc][traverse]" )
{
    std::vector<TraverseMock> mocks(10);

    GC::traverse_container(mocks, 42);

    for (auto& mock : mocks)
    {
        REQUIRE(mock.mark == 42);
    }
}

TEST_CASE( "Traverse array container of Mocks", "[gc][traverse]" )
{
    std::array<TraverseMock, 10> mocks;

    GC::traverse_container(mocks, 42);

    for (auto& mock : mocks)
    {
        REQUIRE(mock.mark == 42);
    }
}

TEST_CASE( "Traverse C array of Mocks", "[gc][traverse]" )
{
    std::array<TraverseMock, 10> mocks;

    GC::traverse<TraverseMock[10]>(mocks, 42);

    for (auto& mock : mocks)
    {
        REQUIRE(mock.mark == 42);
    }
}

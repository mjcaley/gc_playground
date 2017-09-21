#include "catch.hpp"

#include <typeinfo>

#include "GC/GC.hpp"


TEST_CASE( "TypedObject saves type information", "[gc][object]" )
{
    REQUIRE(typeid(GC::TypedObject<int>::type) == typeid(int));
}

TEST_CASE( "TypedObject accepts parameter", "[gc][object]")
{
    auto typed_obj = GC::TypedObject<int>(42);
    REQUIRE(typed_obj.object == 42);
}

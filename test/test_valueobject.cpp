#include "catch.hpp"

#include <memory>
#include <type_traits>

#include "GC/ValueObject.hpp"


TEST_CASE( "ValueObject constructor", "[gc][valueobject]")
{
    GC::ValueObject<int> v(42);

    REQUIRE(v.ptr != nullptr);
    REQUIRE(*v.ptr == 42);
}

TEST_CASE( "ValueObject new_object", "[gc][valueobject]" )
{
    auto v = GC::new_object<int>(42);

    REQUIRE(v->ptr != nullptr);
    REQUIRE(*v->ptr == 42);
}

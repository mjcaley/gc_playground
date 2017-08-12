#include "catch.hpp"

#include <memory>
#include <type_traits>

#include "GC/ValueObject.hpp"


TEST_CASE( "ValueObject constructor", "[gc][valueobject]")
{
    GC::ValueObject<int> v(42);

    auto type_result = std::is_same<decltype(v.ptr), const std::unique_ptr<int>>::value;
    REQUIRE(type_result);
    REQUIRE(v.ptr);
    REQUIRE(*v.ptr == 42);
}

TEST_CASE( "ValueObject new_object", "[gc][valueobject]" )
{
    auto v = GC::new_object<int>(42);

    auto type_result = std::is_same<decltype(v), std::unique_ptr<GC::ValueObject<int>>>::value;
    REQUIRE(type_result);
    REQUIRE(v->ptr);
    REQUIRE(*v->ptr == 42);
}

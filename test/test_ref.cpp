#include "catch.hpp"

#include <string>

#include "GC/GC.hpp"


TEST_CASE( "Ref value not nullptr", "[gc][ref]" )
{
    GC::Ref<int> r(42);

    REQUIRE_FALSE(r.get() == nullptr);
}

TEST_CASE( "Ref dereference operator", "[gc][ref]" )
{
    GC::Ref<int> r(42);

    REQUIRE(*r == 42);
}

TEST_CASE( "Ref operator", "[gc][ref]" )
{
    struct Example
    {
        Example(int first, float second, std::string third) : first(first), second(second), third(third) {}

        int first;
        float second;
        std::string third;
    };

    GC::Ref<Example> r(1, 2.2f, "Example");

    REQUIRE(r->first == 1);
    REQUIRE(r->second == 2.2f);
    REQUIRE(r->third == "Example");
}

TEST_CASE( "Ref copy to WeakRef", "[gc][ref]" )
{
    GC::Ref<int> ref(42);
    GC::WeakRef<int> wref(ref);

    REQUIRE(ref.get()->get_weak_ref_count() == 1);
}

TEST_CASE( "Ref from Ptr", "[gc][ref]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    GC::Ref<int> ref(&ptr);

    REQUIRE(ref.get() == &ptr);
}

TEST_CASE( "Ref copy constructor", "[gc][ref]" )
{
    GC::Ref<int> ref(42);
    auto ref2 = ref;

    REQUIRE(ref.get()->get_ref_count() == 2);
    REQUIRE(ref.get() == ref2.get());
}

TEST_CASE( "Ref from WeakRef", "[gc][ref]" )
{
    GC::Ref<int> orig(42);
    GC::WeakRef<int> weak(orig);
    GC::Ref<int> new_ref(weak);

    REQUIRE(new_ref.get() == orig.get());
}

TEST_CASE( "Ref to WeakRef" , "[gc][ref]" )
{
    GC::Ref<int> orig(42);
    
    REQUIRE_NOTHROW(orig.to_weak_ref());
}

TEST_CASE( "Ref dereferences when out of scope", "[gc][ref]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    {
        GC::Ref<int> ref(&ptr);
        REQUIRE(ptr.get_ref_count() == 1);
    }

    REQUIRE(ptr.get_ref_count() == 0);
}

TEST_CASE( "Ref array type is std::array", "[gc][ref]" )
{
    bool result = std::is_same<GC::Ref<int[3]>::type, std::array<int, 3>>::value;

    REQUIRE(result);
}

TEST_CASE( "Ref array type has subscript operator", "[gc][ref]" )
{
    GC::Ref<int[3]> ref;
    *ref = {1, 2, 3};

    REQUIRE(ref[0] == 1);
    REQUIRE(ref[1] == 2);
    REQUIRE(ref[2] == 3);
}

TEST_CASE( "Ref array type has at member function", "[gc][ref]" )
{
    GC::Ref<int[3]> ref;
    *ref = {1, 2, 3};

    REQUIRE(ref.at(0) == 1);
    REQUIRE(ref.at(1) == 2);
    REQUIRE(ref.at(2) == 3);
}

TEST_CASE( "Ref array type 'at' member function raises std::out_of_range", "[gc][ref]" )
{
    GC::Ref<int[3]> ref;

    REQUIRE_THROWS_AS(ref.at(3), std::out_of_range);
}

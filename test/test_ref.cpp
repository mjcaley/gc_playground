#include "catch.hpp"

#include "GC/Ref.hpp"
#include "GC/ValueObject.hpp"
#include <GC/WeakRef.hpp>


TEST_CASE( "Ref value constructor and value assigned", "[gc][ref]" )
{
    GC::Ref<int> r(42);

    REQUIRE(*r == 42);
}

TEST_CASE( "Ref ValueObject constructor", "[gc][ref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);

    REQUIRE(*r == 42);
    REQUIRE(v.get_ref_count() == 1);
}

TEST_CASE( "Ref copy constructor", "[gc][ref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r1(&v);
    GC::Ref<int> r2(r1);

    REQUIRE(*r1 == 42);
    REQUIRE(*r2 == 42);
    REQUIRE(v.get_ref_count() == 2);
}

TEST_CASE( "Ref destructor dereferences", "[gc][ref]" )
{
    GC::ValueObject<int> v(42);
    {
        GC::Ref<int> r(&v);

        REQUIRE(*r == 42);
        REQUIRE(v.get_ref_count() == 1);
    }

    REQUIRE(v.get_ref_count() == 0);
}

TEST_CASE( "WeakRef from Ref implicit conversion", "[gc][ref][weakref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);
    GC::WeakRef<int> w(r);

    REQUIRE(*r == 42);
    REQUIRE(v.get_ref_count() == 1);
    REQUIRE(v.get_weak_ref_count() == 1);
}

TEST_CASE( "Ref get_weak_reference()", "[gc][ref][weakref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);
    auto w = r.get_weak_reference();

    auto type_check = std::is_same<decltype(w), GC::WeakRef<int>>::value;
    REQUIRE(type_check);
    REQUIRE(*r == 42);
    REQUIRE(v.get_ref_count() == 1);
    REQUIRE(v.get_weak_ref_count() == 1);
}

TEST_CASE( "Ref dereference operator", "[gc][ref]")
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);

    REQUIRE(*r == 42);
}


struct TestObject
{
    int member_variable { 42 };
};

TEST_CASE( "Ref member access operator", "[gc][ref]" )
{
    GC::ValueObject<TestObject> v;
    GC::Ref<TestObject> r(&v);

    REQUIRE(r->member_variable == 42);
}

TEST_CASE( "Ref mark function", "[gc][ref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);

    r.mark(1);

    REQUIRE(v.get_mark() == 1);
}

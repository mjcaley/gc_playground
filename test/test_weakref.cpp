#include "catch.hpp"

#include "GC/Ref.hpp"
#include "GC/ValueObject.hpp"
#include "GC/WeakRef.hpp"


TEST_CASE( "WeakRef default constructor", "[gc][weakref]" )
{
    GC::WeakRef<int> w;
}

TEST_CASE( "WeakRef ValueObject pointer constructor", "[gc][weakref]" )
{
    GC::ValueObject<int> v(42);
    GC::WeakRef<int> w(&v);

    REQUIRE(v.get_weak_ref_count() == 1);
}

TEST_CASE( "WeakRef nullptr constructor", "[gc][weakref]" )
{
    GC::WeakRef<int> w(nullptr);
}

TEST_CASE( "WeakRef copy constructor", "[gc][weakref]" )
{
    GC::ValueObject<int> v(42);
    GC::WeakRef<int> w1(&v);
    GC::WeakRef<int> w2(w1);

    REQUIRE(v.get_weak_ref_count() == 2);
}

TEST_CASE( "WeakRef destructor dereferences", "[gc][weakref]" )
{
    GC::ValueObject<int> v(42);
    {
        GC::WeakRef<int> w(&v);

        REQUIRE(v.get_weak_ref_count() == 1);
    }

    REQUIRE(v.get_weak_ref_count() == 0);
}

TEST_CASE( "WeakRef implicit conversion to Ref", "[gc][weakref][ref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);
    GC::WeakRef<int> w(r);

    REQUIRE(v.get_weak_ref_count() == 1);
}

TEST_CASE( "WeakRef get_reference()", "[gc][weakref]")
{
    GC::ValueObject<int> v(42);
    GC::WeakRef<int> w(&v);
    auto r = w.get_reference();

    REQUIRE(*r == 42);
    REQUIRE(v.get_ref_count() == 1);
}

TEST_CASE( "WeakRef get_reference() throws std::bad_weak_ptr on nullptr", "[gc][weakref]")
{
    GC::WeakRef<int> w;

    REQUIRE_THROWS_AS(w.get_reference(), std::bad_weak_ptr);
}

TEST_CASE( "WeakRef conversion to bool", "[gc][weakref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);
    GC::WeakRef<int> w(r);

    REQUIRE(static_cast<bool>(w));
}

TEST_CASE( "WeakRef has_reference()", "[gc][weakref]" )
{
    GC::ValueObject<int> v(42);
    GC::Ref<int> r(&v);
    GC::WeakRef<int> w(r);

    REQUIRE(w.has_reference() == true);
}

TEST_CASE( "WeakRef has_reference() nullptr", "[gc][weakref]")
{
    GC::WeakRef<int> w;

    REQUIRE(w.has_reference() == false);
}

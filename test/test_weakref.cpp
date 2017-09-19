#include "catch.hpp"

#include "GC/GC.hpp"


TEST_CASE( "WeakRef from Ref", "[gc][weakref]" )
{
    GC::Ref<int> ref(42);
    
    REQUIRE_NOTHROW( [&ref](){ GC::WeakRef<int> weak(ref); }() );
}

TEST_CASE( "WeakRef from Ptr", "[gc][weakref]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    GC::WeakRef<int> weak(&ptr);

    REQUIRE(ptr.get_weak_ref_count() == 1);
}

TEST_CASE( "WeakRef increments weak reference count", "[gc][weakref]" )
{
    GC::Ref<int> ref(42);
    GC::WeakRef<int> weak(ref);

    REQUIRE(ref.get()->get_weak_ref_count() == 1);
}

TEST_CASE( "WeakRef decrements weak reference count when out of scope", "[gc][weakref]" )
{
    GC::Ref<int> ref(42);
    {
        GC::WeakRef<int> weak(ref);
        REQUIRE(ref.get()->get_weak_ref_count() == 1);
    }

    REQUIRE(ref.get()->get_weak_ref_count() == 0);
}

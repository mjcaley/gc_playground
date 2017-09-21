#include "catch.hpp"

#include "GC/GC.hpp"


TEST_CASE( "Create with traverse function", "[gc][ptr]" )
{
    auto traverse_func = [](unsigned int mark) {};
    REQUIRE_NOTHROW(GC::Ptr::create_traverse<int>(traverse_func, 42));
}

TEST_CASE( "Create Ptr", "[gc][ptr]" )
{
    REQUIRE_NOTHROW(GC::Ptr::create<int>(42));
}

TEST_CASE( "Ptr default values", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);

    REQUIRE(ptr.get_mark() == 0);
    REQUIRE(ptr.get_ref_count() == 0);
    REQUIRE(ptr.get_weak_ref_count() == 0);
}

TEST_CASE( "Ptr::get_object returns valid pointer", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);

    REQUIRE(ptr.get_object() != nullptr);
}

TEST_CASE( "Ptr::get_mark returns current mark", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.set_mark(42);

    REQUIRE(ptr.get_mark() == 42);
}

TEST_CASE( "Ptr::get_ref_count returns current reference count", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference();
    
    REQUIRE(ptr.get_ref_count() == 1);
}

TEST_CASE( "Ptr::get_weak_ref_count returns current weak reference count", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference_weak();
    
    REQUIRE(ptr.get_weak_ref_count() == 1);
}

TEST_CASE( "Ptr::is_root false with no references", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    
    REQUIRE_FALSE(ptr.is_root());
}

TEST_CASE( "Ptr::is_root true with reference", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference();
    
    REQUIRE(ptr.is_root());
}

TEST_CASE( "Ptr::is_root false with weak reference", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference_weak();
    
    REQUIRE_FALSE(ptr.is_root());
}

TEST_CASE( "Ptr::reference increments by one", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    auto orig_ref = ptr.get_ref_count();
    ptr.reference();
    
    REQUIRE(ptr.get_ref_count() == orig_ref + 1);
}

TEST_CASE( "Ptr::dereference decrements by one", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference();
    auto orig_ref = ptr.get_ref_count();
    ptr.dereference();
    
    REQUIRE(orig_ref == 1);
    REQUIRE(ptr.get_ref_count() == 0);
}

TEST_CASE( "Ptr::reference_weak increments by one", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    auto orig_ref = ptr.get_weak_ref_count();
    ptr.reference_weak();
    
    REQUIRE(ptr.get_weak_ref_count() == orig_ref + 1);
}

TEST_CASE( "Ptr::dereference_weak decrements by one", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference_weak();
    auto orig_ref = ptr.get_weak_ref_count();
    ptr.dereference_weak();
    
    REQUIRE(orig_ref == 1);
    REQUIRE(ptr.get_weak_ref_count() == 0);
}

TEST_CASE( "Ptr::mark sets mark to value", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.mark(42);
    
    REQUIRE(ptr.get_mark() == 42);
}

TEST_CASE( "Ptr::mark calls traversal function", "[gc][ptr]" )
{
    bool trigger { false };
    auto traverse_func = [&trigger](unsigned int mark) { trigger = true; };

    auto ptr = GC::Ptr::create_traverse<int>(traverse_func, 42);
    ptr.mark(GC::Detail::current_mark + 1);

    REQUIRE(trigger == true);
}

TEST_CASE( "Ptr frees pointer when reference count decrements to zero", "[gc][ptr]" )
{
    auto ptr = GC::Ptr::create<int>(42);
    ptr.reference();
    ptr.dereference();
    auto* object = ptr.get_object();

    REQUIRE(object == nullptr);
}

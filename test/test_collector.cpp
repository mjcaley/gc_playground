#include "catch.hpp"

#include "GC/GC.hpp"


TEST_CASE( "Allocations existing zeroed", "[gc][collector]" )
{
    GC::collect();
    REQUIRE(GC::allocations.existing == 0);
}

TEST_CASE( "Allocations existing increments when object added", "[gc][collector]" )
{
    GC::Ref<int> ref(42);
    GC::collect();

    REQUIRE(GC::allocations.existing == 1);
}

TEST_CASE( "Allocations created zeroed", "[gc][collector]" )
{
    GC::collect();
    REQUIRE(GC::allocations.created == 0);
}

TEST_CASE( "Allocations created increments when object added", "[gc][collector]" )
{
    GC::collect();
    auto old_percentage = GC::collection_percentage;
    GC::collection_percentage = 0.0f;  // Make sure collection isn't done automatically
    GC::Ref<int> ref(42);
    GC::collection_percentage = old_percentage;

    REQUIRE(GC::allocations.created == 1);
}

TEST_CASE( "Allocations deleted zeroed", "[gc][collector]" )
{
    GC::collect();
    REQUIRE(GC::allocations.deleted == 0);
}

TEST_CASE( "Allocations deleted increments when object deleted", "[gc][collector]" )
{
    GC::collect();
    {
        GC::Ref<int> ref(42);
    }
    GC::mark(GC::Detail::current_mark);
    GC::sweep(GC::Detail::current_mark);

    REQUIRE(GC::allocations.deleted == 1);
}

TEST_CASE( "Allocations resolved", "[gc][collector]" )
{
    auto orig_existing = GC::allocations.existing;
    auto orig_created = GC::allocations.created;
    auto orig_deleted = GC::allocations.deleted;

    GC::allocations.existing = 0;
    GC::allocations.created = 10;
    GC::allocations.deleted = 0;
    GC::allocations.resolve();
    REQUIRE(GC::allocations.existing == 10);
    REQUIRE(GC::allocations.created == 0);
    REQUIRE(GC::allocations.deleted == 0);

    GC::allocations.existing = 10;
    GC::allocations.created = 0;
    GC::allocations.deleted = 10;
    GC::allocations.resolve();
    REQUIRE(GC::allocations.existing == 0);
    REQUIRE(GC::allocations.created == 0);
    REQUIRE(GC::allocations.deleted == 0);

    GC::allocations.existing = orig_existing;
    GC::allocations.created = orig_created;
    GC::allocations.deleted = orig_deleted;
}

TEST_CASE( "GC::mark marks objects with current mark", "[gc][collector]" )
{
    GC::collect();
    GC::Ref<int> ref(42);
    GC::mark(GC::Detail::current_mark);
    for(auto& obj : GC::Detail::used_list)
    {
        REQUIRE(obj.get_mark() == GC::Detail::current_mark);
    }
}

TEST_CASE( "GC::sweep deletes all objects", "[gc][collector]" )
{
    GC::collect();
    {
        GC::Ref<int> ref(42);
    }
    ++GC::Detail::current_mark;  // Make sure mark is different
    
    REQUIRE_FALSE(GC::Detail::used_list.empty());
    GC::sweep(GC::Detail::current_mark);
    REQUIRE(GC::Detail::used_list.empty());
}

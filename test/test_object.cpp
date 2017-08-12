#include "catch.hpp"

#include <functional>

#include "GC/Object.hpp"


TEST_CASE( "Object default values", "[gc][object]" )
{
    GC::Object o;

    REQUIRE_FALSE(o.is_root());
    REQUIRE(o.get_ref_count() == 0);
    REQUIRE(o.get_mark() == 0);
}

TEST_CASE( "Object reference increments", "[gc][object]" )
{
    GC::Object o;

    REQUIRE(o.get_ref_count() == 0);
    o.reference();
    REQUIRE(o.get_ref_count() == 1);
}

TEST_CASE( "Object reference decrements", "[gc][object]" )
{
    GC::Object o;

    REQUIRE(o.get_ref_count() == 0);
    o.reference();
    o.dereference();
    REQUIRE(o.get_ref_count() == 0);
}

TEST_CASE( "Object weak reference increments", "[gc][object]" )
{
    GC::Object o;

    REQUIRE(o.get_weak_ref_count() == 0);
    o.reference_weak();
    REQUIRE(o.get_weak_ref_count() == 1);
}

TEST_CASE( "Object weak dereference decrements", "[gc][object]" )
{
    GC::Object o;

    REQUIRE(o.get_weak_ref_count() == 0);
    o.reference_weak();
    o.dereference_weak();
    REQUIRE(o.get_weak_ref_count() == 0);
}

TEST_CASE( "Object mark sets", "[gc][object]")
{
    GC::Object o;

    REQUIRE(o.get_mark() == 0);
    o.set_mark(42);
    REQUIRE(o.get_mark() == 42);
}

TEST_CASE( "Object mark function", "[gc][object]" )
{
    GC::Object o;

    unsigned int MARK { 42 };
    o.traverse_func = [](unsigned int mark) {};

    o.mark(MARK);

    REQUIRE(o.get_mark() == MARK);
}

void test_traverse(bool* result, unsigned int* mark_called, unsigned int mark)
{
    *result = true;
    *mark_called = mark;
}

TEST_CASE( "Object traverse function", "[gc][object]" )
{
    GC::Object o;

    bool traverse_ran { false };
    unsigned int mark_result { 0 };
    unsigned int MARK { 42 };

    o.traverse_func = std::bind(test_traverse,
                                &traverse_ran,
                                &mark_result,
                                std::placeholders::_1);
    o.mark(MARK);

    REQUIRE(traverse_ran);
    REQUIRE(mark_result == MARK);
}

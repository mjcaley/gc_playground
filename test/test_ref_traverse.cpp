#include "catch.hpp"

#include "GC/GC.hpp"


struct RefTraverseMock
{
    unsigned int mark { 0 };
};

template<>
void GC::traverse(RefTraverseMock& object, unsigned int marker)
{
    object.mark = marker;
}

TEST_CASE( "Traverse reference to RefTraverseMock", "[gc][traverse]" )
{
    GC::Ref<RefTraverseMock> ref;
    GC::traverse(ref, 42);

    REQUIRE(ref->mark == 42);
}

TEST_CASE( "Traverse weak reference to RefTraverseMock", "[gc][traverse]" )
{
    GC::Ref<RefTraverseMock> ref;
    GC::WeakRef<RefTraverseMock> weak(ref);
    GC::traverse(weak, 42);

    REQUIRE(ref->mark == 42);
}

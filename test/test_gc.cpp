#include "catch.hpp"

#include "GC/GC.hpp"
#include "GC/Ref.hpp"
#include "GC/ValueObject.hpp"


TEST_CASE( "GC create object and get reference", "[gc]" )
{
    auto r = GC::GC::create<int>(42);

    REQUIRE(*r == 42);
}

TEST_CASE( "GC get_roots returns reference", "[gc]" )
{
    auto v = GC::new_object<int>(42);
    auto* v_ptr = v.get();
    GC::Ref<int> r(v_ptr);

    GC::GC::add_to_gc(std::move(v));

    auto roots = GC::GC::get_roots();

    REQUIRE(roots[0] == v_ptr);
}

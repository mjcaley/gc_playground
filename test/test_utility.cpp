#include "catch.hpp"

#include <array>

#include "GC/Ref.hpp"
#include "GC/Utility.hpp"
#include "GC/WeakRef.hpp"


TEST_CASE( "Convert array of Ref to WeakRef", "[gc][utility][ref][weakref]")
{
    std::array<GC::Ref<int>, 5> from { 1, 2, 3, 4, 5 };
    auto to = GC::convert(from);
}

TEST_CASE( "Convert array of WeakRef to Ref", "[gc][utility][ref][weakref]")
{
    auto one   = GC::Ref<int>(1);
    auto two   = GC::Ref<int>(2);
    auto three = GC::Ref<int>(3);
    auto four  = GC::Ref<int>(4);
    auto five  = GC::Ref<int>(5);
    std::array<GC::WeakRef<int>, 5> from { GC::WeakRef<int>(one),
                                           GC::WeakRef<int>(two),
                                           GC::WeakRef<int>(three),
                                           GC::WeakRef<int>(four),
                                           GC::WeakRef<int>(five) };

    auto to = GC::convert(from);

    REQUIRE(*to[0] == 1);
    REQUIRE(*to[1] == 2);
    REQUIRE(*to[2] == 3);
    REQUIRE(*to[3] == 4);
    REQUIRE(*to[4] == 5);
}



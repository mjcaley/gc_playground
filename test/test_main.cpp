#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "GC/Object.hpp"


TEST_CASE( "Object default values", "[gc][object]" )
{
    GC::Object o;
    REQUIRE(o.is_root() == false);
}


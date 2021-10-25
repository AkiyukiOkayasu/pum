#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "pum.hpp"

#include <doctest/doctest.h>
#include <iomanip>

using doctest::Approx;

TEST_CASE ("Test")
{
    SUBCASE ("test2")
    {
        CHECK (1.0f == Approx (1.0f));
    }
}

#include "scope_guard.h"
#include <catch.hpp>

TEST_CASE("deleter called on destruction", "[ScopeGuard]")
{
    bool executed = false;

    {
        auto guard = sg::scope_guard([&executed] { executed = true; });
        static_cast<void>(guard);
    }

    REQUIRE(executed == true);
}

TEST_CASE("deleter is not called if released", "[ScopeGuard]")
{
    bool executed = false;

    {
        auto guard = sg::scope_guard([&executed] { executed = true; });
        guard.release();
    }

    REQUIRE(executed == false);
}

TEST_CASE("move releases moved-from object", "[ScopeGuard]")
{
    std::size_t calls{0};

    {
        auto movedFrom = sg::scope_guard([&calls] { ++calls; });
        auto guard = std::move(movedFrom);

    }

    REQUIRE(calls == 1);
}

TEST_CASE("move transfers state", "[ScopeGuard]")
{
    bool executed = false;

    {
        auto movedFrom = sg::scope_guard([&executed] { executed = true; });
        auto guard = std::move(movedFrom);
    }

    REQUIRE(executed == true);
}

TEST_CASE("move transfers state if released", "[ScopeGuard]")
{
    bool executed = false;

    {
        auto movedFrom = sg::scope_guard([&executed] { executed = true; });
        movedFrom.release();
        auto guard = std::move(movedFrom);
    }

    REQUIRE(executed == false);
}

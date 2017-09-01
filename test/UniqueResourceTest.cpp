/*
 * Scope Guard
 * Copyright (C) 2017  offa
 *
 * This file is part of Scope Guard.
 *
 * Scope Guard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Scope Guard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scope Guard.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "unique_resource.h"
#include <catch.hpp>

using Handle = int;

TEST_CASE("deleter called on destruction", "[UniqueResource]")
{
    std::size_t calls{0};
    constexpr Handle handle{3};

    {
        auto guard = sr::unique_resource(handle, [&calls] { ++calls; });
        static_cast<void>(guard);
    }

    REQUIRE(calls == 1);
}

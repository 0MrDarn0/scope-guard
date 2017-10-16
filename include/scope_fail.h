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

#pragma once

#include "scope_guard_base.h"
#include <exception>

namespace sr
{
    namespace detail
    {

        struct scope_fail_strategy
        {
            bool should_execute() const noexcept
            {
                return uncaught_exceptions() > m_uncaught_on_creation;
            }

            int uncaught_exceptions() const noexcept
            {
                return ( std::uncaught_exception() == true ? 1 : 0 );
            }


            int m_uncaught_on_creation = uncaught_exceptions();
        };

    }


    template<class EF>
    class scope_fail : public detail::scope_guard_base<EF, detail::scope_fail_strategy>
    {
    public:

        using detail::scope_guard_base<EF, detail::scope_fail_strategy>::scope_guard_base;


    private:

    };


    template<class EF>
    scope_fail<std::decay_t<EF>> make_scope_fail(EF&& exitFunction)
    {
        return scope_fail<std::decay_t<EF>>{std::forward<EF>(exitFunction)};
    }

}


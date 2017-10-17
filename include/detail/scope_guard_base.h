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

#include <utility>
#include <type_traits>

namespace sr
{
namespace detail
{

    template<class F, class S>
    struct is_noexcept_dtor
    {
        static constexpr bool value = true;
    };


    template<class F, class S>
    constexpr bool is_noexcept_dtor_v = is_noexcept_dtor<F, S>::value;



    template<class EF, class Strategy>
    class scope_guard_base : private Strategy
    {
    public:

        template<class EFP,
            std::enable_if_t<std::is_constructible<EF, EFP>::value, int> = 0,
            std::enable_if_t<(!std::is_lvalue_reference<EFP>::value)
                                && std::is_nothrow_constructible<EF, EFP>::value, int> = 0
            >
        explicit scope_guard_base(EFP&& exitFunction) : m_exitFunction(std::move(exitFunction)),
                                                    m_execute_on_destruction(true)
        {
        }

        template<class EFP,
            std::enable_if_t<std::is_constructible<EF, EFP>::value, int> = 0,
            std::enable_if_t<std::is_lvalue_reference<EFP>::value, int> = 0
            >
        explicit scope_guard_base(EFP&& exitFunction) try : m_exitFunction(exitFunction),
                                                        m_execute_on_destruction(true)
        {
        }
        catch( ... )
        {
            exitFunction();
            throw;
        }

        scope_guard_base(scope_guard_base&& other) noexcept(std::is_nothrow_move_constructible<EF>::value
                                                            || std::is_nothrow_copy_constructible<EF>::value)
                                        : Strategy(other),
                                        m_exitFunction(std::move_if_noexcept(other.m_exitFunction)),
                                        m_execute_on_destruction(other.m_execute_on_destruction)
        {
            other.release();
        }

        scope_guard_base(const scope_guard_base&) = delete;


        ~scope_guard_base() noexcept(is_noexcept_dtor_v<EF, Strategy>)
        {
            if( (m_execute_on_destruction == true) && (this->should_execute() == true) )
            {
                m_exitFunction();
            }
        }


        void release() noexcept
        {
            m_execute_on_destruction = false;
        }


        scope_guard_base& operator=(const scope_guard_base&) = delete;
        scope_guard_base& operator=(scope_guard_base&&) = delete;


    private:

        EF m_exitFunction;
        bool m_execute_on_destruction;
    };

}
}

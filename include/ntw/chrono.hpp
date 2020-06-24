/*
 * Copyright 2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <chrono>
#include <cstdint>
#include "detail/common.hpp"
#include "status.hpp"

namespace ntw {

    struct clock {
        using rep                       = std::int64_t;
        using period                    = std::ratio<1, 10000000>;
        using duration                  = std::chrono::duration<rep, period>;
        using time_point                = std::chrono::time_point<clock>;
        constexpr static bool is_steady = false; // not sure about this one to be honest

        /// \brief Returns current system time.
        NTW_INLINE static time_point now() noexcept;
    };

    using nanosecond_hundreds = std::ratio<1, 10000000>;
    using duration            = clock::duration;
    using time_point          = clock::time_point;

    struct alertable_t {
        explicit constexpr alertable_t() = default;
    };

    constexpr inline alertable_t alertable;

    /// \brief Sleeps for given delay in non alertable state.
    NTW_INLINE status sleep_for(duration dur) noexcept;

    /// \brief Sleeps for given delay in alertable state.
    NTW_INLINE status sleep_for(duration dur, alertable_t) noexcept;

    /// \brief Sleeps for until given time in non alertable state.
    NTW_INLINE status sleep_until(time_point tp) noexcept;

    /// \brief Sleeps for until given time in alertable state.
    NTW_INLINE status sleep_until(time_point tp, alertable_t) noexcept;

} // namespace ntw

#include "impl/chrono.inl"
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

#include "../chrono.hpp"

namespace ntw {

    NTW_INLINE time_point clock::now() noexcept
    {
        return time_point{ duration{
            *reinterpret_cast<volatile std::int64_t*>(&USER_SHARED_DATA->SystemTime) } };
    }

    NTW_INLINE status sleep_for(duration dur) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = -dur.count();
        return NTW_SYSCALL(NtDelayExecution)(false, &interval);
    }

    NTW_INLINE status sleep_for(duration dur, alertable_t) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = -dur.count();
        return NTW_SYSCALL(NtDelayExecution)(true, &interval);
    }

    NTW_INLINE status sleep_until(time_point tp) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = tp.time_since_epoch().count();
        return NTW_SYSCALL(NtDelayExecution)(false, &interval);
    }

    NTW_INLINE status sleep_until(time_point tp, alertable_t) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = tp.time_since_epoch().count();
        return NTW_SYSCALL(NtDelayExecution)(true, &interval);
    }

} // namespace ntw
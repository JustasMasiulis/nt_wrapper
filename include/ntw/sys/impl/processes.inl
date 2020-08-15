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

#include "../processes.hpp"

namespace ntw::sys {

    static_assert(sizeof(thread) == sizeof(SYSTEM_THREAD_INFORMATION));
    static_assert(sizeof(process) ==
                  (sizeof(SYSTEM_PROCESS_INFORMATION) - sizeof(thread)));

    template<class Range>
    NTW_INLINE ntw::result<process::range_type> processes(Range&&  buffer,
                                                          ulong_t* returned)
    {
        const auto  first  = detail::unfancy(detail::adl_begin(buffer));
        const auto  size   = static_cast<ulong_t>(detail::range_byte_size(buffer));
        ntw::status status = NTW_SYSCALL(NtQuerySystemInformation)(
            SystemProcessInformation, first, size, returned);

        return { status, { reinterpret_cast<process*>(first) } };
    }

    NTW_INLINE std::span<thread> process::threads() noexcept
    {
        return { reinterpret_cast<thread*>(this + 1), thread_count };
    }

    NTW_INLINE std::span<const thread> process::threads() const noexcept
    {
        return { reinterpret_cast<const thread*>(this + 1), thread_count };
    }

} // namespace ntw::sys
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
#include "../pool_tags.hpp"

namespace ntw::sys {

    static_assert(sizeof(system_pooltag) == sizeof(SYSTEM_POOLTAG));

    template<class Range>
    NTW_INLINE ::ntw::result<std::span<system_pooltag>>
    pool_tags(Range&& buffer, ulong_t* returned) noexcept
    {
        const auto  first  = ::ntw::detail::unfancy(::ntw::detail::adl_begin(buffer));
        const auto  size   = static_cast<ulong_t>(::ntw::detail::range_byte_size(buffer));
        ntw::status status = NTW_SYSCALL(NtQuerySystemInformation)(
            SystemPoolTagInformation, first, size, returned);

        const auto info = reinterpret_cast<SYSTEM_POOLTAG_INFORMATION*>(first);
        return { status,
                 std::span<system_pooltag>{
                     reinterpret_cast<system_pooltag*>(info->TagInfo), info->Count } };
    }

} // namespace ntw::sys
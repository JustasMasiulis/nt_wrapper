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
#include "../modules.hpp"

namespace ntw::sys {

    NTW_INLINE const char* loaded_module::name() const noexcept
    {
        return path + file_name_offset;
    }

    NTW_INLINE std::string_view loaded_module::name_view() const noexcept
    {
        const auto first = name();
        auto       last  = first;
        while(*last)
            ++last;

        return std::string_view(first,
                                static_cast<std::string_view::size_type>(last - first));
    }

    NTW_INLINE std::string_view loaded_module::path_view() const noexcept
    {
        auto driver_name = name_view();
        return { driver_name.data() - file_name_offset,
                 driver_name.size() + file_name_offset };
    }

    template<class Range>
    ntw::result<loaded_module::range_type> loaded_modules(Range&& buffer)
    {
        const auto  first  = detail::unfancy(detail::adl_begin(buffer));
        const auto  size   = static_cast<ulong_t>(detail::range_byte_size<Range>(buffer));
        ntw::status status = NTW_SYSCALL(NtQuerySystemInformation)(
            SystemModuleInformationEx, first, size, nullptr);

        return { status, { reinterpret_cast<loaded_module*>(first) } };
    }

} // namespace ntw::sys
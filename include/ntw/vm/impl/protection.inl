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
#include "../protection.hpp"

namespace ntw::vm {

    NTW_INLINE constexpr protection::protection(std::uint32_t raw_value) noexcept
        : value(raw_value){};

    NTW_INLINE constexpr std::uint32_t protection::get() const noexcept { return value; }

    NTW_INLINE constexpr bool protection::accessible() const noexcept
    {
        return !(value & PAGE_NOACCESS);
    }

    NTW_INLINE constexpr bool protection::readable() const noexcept
    {
        return value &
               (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
                PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
    }

    NTW_INLINE constexpr bool protection::writeable() const noexcept
    {
        return value & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE |
                        PAGE_EXECUTE_WRITECOPY);
    }

    NTW_INLINE constexpr bool protection::executeable() const noexcept
    {
        return value & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                        PAGE_EXECUTE_WRITECOPY);
    }

    NTW_INLINE constexpr bool protection::guarded() const noexcept
    {
        return value & PAGE_GUARD;
    }

    NTW_INLINE constexpr bool protection::non_cached() const noexcept
    {
        return value & PAGE_NOCACHE;
    }

    NTW_INLINE constexpr bool protection::write_combined() const noexcept
    {
        return value & PAGE_WRITECOMBINE;
    }

    NTW_INLINE constexpr protection protection::no_access() noexcept
    {
        return { PAGE_NOACCESS };
    }

    NTW_INLINE constexpr protection protection::read() noexcept
    {
        return { PAGE_READONLY };
    }

    NTW_INLINE constexpr protection protection::execute() noexcept
    {
        return { PAGE_EXECUTE };
    }

    NTW_INLINE constexpr protection protection::read_write() noexcept
    {
        return { PAGE_READWRITE };
    }

    NTW_INLINE constexpr protection protection::read_writecopy() noexcept
    {
        return { PAGE_WRITECOPY };
    }

    NTW_INLINE constexpr protection protection::read_execute() noexcept
    {
        return { PAGE_EXECUTE_READ };
    }

    NTW_INLINE constexpr protection protection::read_write_execute() noexcept
    {
        return { PAGE_EXECUTE_READWRITE };
    }

    NTW_INLINE constexpr protection protection::read_writecopy_execute() noexcept
    {
        return { PAGE_EXECUTE_WRITECOPY };
    }

    NTW_INLINE constexpr protection& protection::guard() noexcept
    {
        value |= PAGE_GUARD;
        return *this;
    }

    NTW_INLINE constexpr protection& protection::disable_caching() noexcept
    {
        value |= PAGE_NOCACHE;
        return *this;
    }

    NTW_INLINE constexpr protection& protection::combine_writes() noexcept
    {
        value |= PAGE_NOCACHE;
        return *this;
    }

} // namespace ntw::vm
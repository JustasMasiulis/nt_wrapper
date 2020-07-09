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
#include "../allocation.hpp"
#include "../../detail/unwrap.hpp"
#include "../../detail/common.hpp"

namespace ntw::vm {

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::commit(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        void*    addr     = _address;
        SIZE_T   win_size = size;
        NTSTATUS result =
            NTW_SYSCALL(NtAllocateVirtualMemory)(::ntw::detail::unwrap(process),
                                                 &addr,
                                                 _zero,
                                                 &win_size,
                                                 _type | MEM_COMMIT,
                                                 prot.get());
        return { result, addr };
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::reserve(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        void*    addr     = _address;
        SIZE_T   win_size = size;
        NTSTATUS result =
            NTW_SYSCALL(NtAllocateVirtualMemory)(::ntw::detail::unwrap(process),
                                                 &addr,
                                                 _zero,
                                                 &win_size,
                                                 _type | MEM_RESERVE,
                                                 prot.get());
        return { result, addr };
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::commit_reserve(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        void*    addr     = _address;
        SIZE_T   win_size = size;
        NTSTATUS result =
            NTW_SYSCALL(NtAllocateVirtualMemory)(::ntw::detail::unwrap(process),
                                                 &addr,
                                                 _zero,
                                                 &win_size,
                                                 _type | MEM_COMMIT | MEM_RESERVE,
                                                 prot.get());
        return { result, addr };
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::zero_bits(
        std::size_t new_ammount) noexcept
    {
        _zero = new_ammount;
        return *this;
    }

    template<class Address>
    NTW_INLINE constexpr allocation_builder&
    allocation_builder::at(Address new_address) noexcept
    {
        _address = const_cast<void*>(reinterpret_cast<const void*>(new_address));
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::top_down() noexcept
    {
        _type |= MEM_TOP_DOWN;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::write_watch() noexcept
    {
        _type |= MEM_WRITE_WATCH;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::physical() noexcept
    {
        _type |= MEM_PHYSICAL;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::rotate() noexcept
    {
        _type |= MEM_ROTATE;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::large_pages() noexcept
    {
        _type |= MEM_LARGE_PAGES;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder allocate() noexcept { return {}; }

} // namespace ntw::vm
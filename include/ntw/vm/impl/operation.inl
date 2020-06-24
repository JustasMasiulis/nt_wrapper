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
#include "../operation.hpp"
#include "../../detail/unwrap.hpp"

namespace ntw::vm {

    template<class Address, class Process>
    NTW_INLINE status reset(Address        address,
                            std::size_t    size,
                            const Process& process) noexcept
    {
        SIZE_T s = size;
        void*  a = const_cast<void*>(reinterpret_cast<const void*>(address));
        return NTW_SYSCALL(NtAllocateVirtualMemory)(
            ::ntw::detail::unwrap(process), &a, 0, &s, MEM_RESET, PAGE_NOACCESS);
    }

    template<class Address, class Process>
    NTW_INLINE status undo_reset(Address        address,
                                 std::size_t    size,
                                 const Process& process) noexcept
    {
        SIZE_T s = size;
        void*  a = const_cast<void*>(reinterpret_cast<const void*>(address));
        return NTW_SYSCALL(NtAllocateVirtualMemory)(
            ::ntw::detail::unwrap(process), &a, 0, &s, MEM_RESET_UNDO, PAGE_NOACCESS);
    }

    template<class Address, class Process>
    NTW_INLINE status release(Address addr, const Process& process) noexcept
    {
        auto   casted = const_cast<void*>(reinterpret_cast<const void*>(addr));
        SIZE_T size   = 0;

        return NTW_SYSCALL(NtFreeVirtualMemory)(
            ::ntw::detail::unwrap(process), &casted, &size, MEM_RELEASE);
    }

    template<class Address, class Process>
    NTW_INLINE status decommit(Address        addr,
                               std::size_t    size,
                               const Process& process) noexcept
    {
        auto   casted   = const_cast<void*>(reinterpret_cast<const void*>(addr));
        SIZE_T win_size = size;

        return NTW_SYSCALL(NtFreeVirtualMemory)(
            ::ntw::detail::unwrap(process), &casted, &win_size, MEM_DECOMMIT);
    }

    template<class Address, class Process>
    NTW_INLINE status unmap(Address addr, const Process& process) noexcept
    {
        auto casted = const_cast<void*>(reinterpret_cast<const void*>(addr));
        return NTW_SYSCALL(NtUnmapViewOfSection)(::ntw::detail::unwrap(process), &casted);
    }

} // namespace ntw::vm
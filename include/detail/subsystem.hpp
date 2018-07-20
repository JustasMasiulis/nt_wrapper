/*
 * Copyright 2018 Justas Masiulis
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
#include "common.hpp"

namespace ntw::detail {

    namespace defs {

        using NtWow64ReadWriteT = NTSTATUS(__stdcall*)(void*          ProcessHandle,
                                                       std::uint64_t  BaseAddress,
                                                       void*          Buffer,
                                                       std::uint64_t  BufferSize,
                                                       std::uint64_t* NumberOfBytesRead);

        using NtWow64ReadVirtualMemory64  = NtWow64ReadWriteT;
        using NtWow64WriteVirtualMemory64 = NtWow64ReadWriteT;

    } // namespace defs

    template<class TargetPointer = std::uintptr_t>
    struct native_subsystem {
        using target_pointer = TargetPointer;

        template<class T>
        NT_FN static read(void*          handle,
                          std::uintptr_t address,
                          T&             buffer,
                          std::size_t    size = sizeof(T)) noexcept
        {
            return LI_NT(NtReadVirtualMemory)(
                handle, reinterpret_cast<void*>(address), &buffer, size, nullptr);
        }

        template<class T>
        NT_FN static write(void*          handle,
                           std::uintptr_t address,
                           const T&       buffer,
                           std::size_t    size = sizeof(T)) noexcept
        {
            return LI_NT(NtWriteVirtualMemory)(handle,
                                               reinterpret_cast<void*>(address),
                                               const_cast<void*>(&buffer),
                                               size,
                                               nullptr);
        }
    };

    struct wow64_subsystem {
        using target_pointer = std::uint64_t;

        template<class T>
        NT_FN static read(void*         handle,
                          std::uint64_t address,
                          T&            buffer,
                          std::uint64_t size = sizeof(T)) noexcept
        {
            using namespace detail::defs;
            return LI_NT_DEF(NtWow64ReadVirtualMemory64)(
                handle, address, static_cast<void*>(&buffer), size, nullptr);
        }

        template<class T>
        NT_FN static write(void*         handle,
                           std::uint64_t address,
                           const T&      buffer,
                           std::uint64_t size = sizeof(T)) noexcept
        {
            using namespace detail::defs;
            return LI_NT_DEF(NtWow64WriteVirtualMemory64)(
                handle,
                address,
                const_cast<void*>(static_cast<const void*>(&buffer)),
                size,
                nullptr);
        }
    };

    template<class Fn, class... Args>
    NT_FN subsystem_call(bool same_arch, Fn fn, Args&&... args) noexcept
    {
        if(same_arch)
            return fn.template operator()<native_subsystem<>>(
                std::forward<Args>(args)...);
#ifdef _WIN64
        return fn.template operator()<native_subsystem<std::uint32_t>>(
            std::forward<Args>(args)...);
#else
        return fn.template operator()<wow64_subsystem>(std::forward<Args>(args)...);
#endif
    }

} // namespace ntw::detail

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
#include "../status.hpp"

namespace ntw::vm {

    template<class Address, class Process = void*>
    NTW_INLINE status reset(Address        address,
                            std::size_t    size,
                            const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status undo_reset(Address        address,
                                 std::size_t    size,
                                 const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status release(Address        addr,
                              const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status decommit(Address        addr,
                               std::size_t    size    = 0,
                               const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status unmap(Address        addr,
                            const Process& process = NtCurrentProcess()) noexcept;

} // namespace ntw::vm

#include "impl/operation.inl"
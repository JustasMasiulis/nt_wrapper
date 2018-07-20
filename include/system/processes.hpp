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
#include "../memory.hpp"
#include "../detail/offset_iterator.hpp"

namespace ntw::system {

    namespace detail {

        struct processes_iterator_traits;
        class threads_list;

    } // namespace detail

    struct thread_info {
        SYSTEM_THREAD_INFORMATION _info;

    public:
        NTW_INLINE void* start_address() const noexcept;

        NTW_INLINE void* id() const noexcept;
        NTW_INLINE void* process_id() const noexcept;

        NTW_INLINE unsigned long state() const noexcept;
    };

    class process_info {
        SYSTEM_PROCESS_INFORMATION _info;

    public:
        NTW_INLINE unsigned long num_threads() const noexcept;

        NTW_INLINE const UNICODE_STRING name() const noexcept;

        NTW_INLINE void* id() const noexcept;
        NTW_INLINE void* parent_id() const noexcept;

        NTW_INLINE detail::threads_list threads() noexcept;

        NTW_INLINE std::int64_t private_working_set() const noexcept;
        NTW_INLINE SIZE_T working_set() const noexcept;

        NTW_INLINE SIZE_T virtual_size() const noexcept;

        NTW_INLINE unsigned long handle_count() const noexcept;

        NTW_INLINE SYSTEM_PROCESS_INFORMATION& get() noexcept;
        NTW_INLINE const SYSTEM_PROCESS_INFORMATION& get() const noexcept;
    };

    class processes {
        memory::unique_alloc<SYSTEM_PROCESS_INFORMATION> _info_buffer;
        unsigned long                                    _buffer_size = 0;

    public:
        using value_type = process_info;
        using iterator = ntw::detail::offset_iterator<detail::processes_iterator_traits>;

        NT_FN update() noexcept;

        NTW_INLINE iterator begin() const noexcept;

        NTW_INLINE iterator end() const noexcept;
    };

} // namespace ntw::system

#include "../impl/processes.inl"
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
#include "../system/processes.hpp"
#include "../system/info.hpp"

namespace ntw::system {
    namespace detail {

        struct processes_iterator_traits {
            using value_type = process_info;

            static unsigned long next_entry_offset(value_type* val)
            {
                return val->get().NextEntryOffset;
            }
        };

        class threads_list {
            SYSTEM_THREAD_INFORMATION* _first;
            SYSTEM_THREAD_INFORMATION* _last;

        public:
            NTW_INLINE constexpr threads_list(SYSTEM_THREAD_INFORMATION* first,
                                              SYSTEM_THREAD_INFORMATION* last) noexcept
                : _first(first), _last(last)
            {}

            NTW_INLINE thread_info* begin() const noexcept
            {
                return reinterpret_cast<thread_info*>(_first);
            }

            NTW_INLINE thread_info* end() const noexcept
            {
                return reinterpret_cast<thread_info*>(_last);
            }
        };

    } // namespace detail


    NTW_INLINE void* thread_info::start_address() const noexcept
    {
        return _info.StartAddress;
    }

    NTW_INLINE void* thread_info::id() const noexcept
    {
        return _info.ClientId.UniqueThread;
    }

    NTW_INLINE void* thread_info::process_id() const noexcept
    {
        return _info.ClientId.UniqueProcess;
    }

    NTW_INLINE unsigned long thread_info::state() const noexcept
    {
        return _info.ThreadState;
    }


    NTW_INLINE unsigned long process_info::num_threads() const noexcept
    {
        return _info.NumberOfThreads;
    }

    NTW_INLINE const UNICODE_STRING process_info::name() const noexcept
    {
        return _info.ImageName;
    }

    NTW_INLINE void* process_info::id() const noexcept { return _info.UniqueProcessId; }
    NTW_INLINE void* process_info::parent_id() const noexcept
    {
        return _info.InheritedFromUniqueProcessId;
    }

    NTW_INLINE SYSTEM_PROCESS_INFORMATION& process_info::get() noexcept { return _info; }
    NTW_INLINE const SYSTEM_PROCESS_INFORMATION& process_info::get() const noexcept
    {
        return _info;
    }

    NTW_INLINE detail::threads_list process_info::threads() noexcept
    {
        return { _info.Threads, _info.Threads + _info.NumberOfThreads };
    }

    NTW_INLINE std::int64_t process_info::private_working_set() const noexcept
    {
        return _info.WorkingSetPrivateSize.QuadPart;
    }

    NTW_INLINE SIZE_T process_info::working_set() const noexcept
    {
        return _info.WorkingSetSize;
    }

    NTW_INLINE SIZE_T process_info::virtual_size() const noexcept
    {
        return _info.VirtualSize;
    }

    NTW_INLINE unsigned long process_info::handle_count() const noexcept
    {
        return _info.HandleCount;
    }


    NT_FN processes::update() noexcept
    {
        return system::update_info(SystemProcessInformation, _info_buffer, _buffer_size);
    }

    NTW_INLINE processes::iterator processes::begin() const noexcept
    {
        return { reinterpret_cast<value_type*>(_info_buffer.get()) };
    }

    NTW_INLINE processes::iterator processes::end() const noexcept { return {}; }

} // namespace ntw::system
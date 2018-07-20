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
#include "info.hpp"

namespace ntw::system {

    class handle_info {
        SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX _entry;

    public:
        NTW_INLINE void* object() const noexcept { return _entry.Object; }

        NTW_INLINE void* value() const noexcept
        {
            return reinterpret_cast<void*>(_entry.HandleValue);
        }

        NTW_INLINE std::uintptr_t pid() const noexcept { return _entry.UniqueProcessId; }

        NTW_INLINE unsigned long access() const noexcept { return _entry.GrantedAccess; }

        NTW_INLINE unsigned short type_index() const noexcept
        {
            return _entry.ObjectTypeIndex;
        }

        NTW_INLINE unsigned long attributes() const noexcept
        {
            return _entry.HandleAttributes;
        }

        NTW_INLINE bool is_thread() const noexcept { return type_index() == 8; }
        NTW_INLINE bool is_process() const noexcept { return type_index() == 7; }
    };

    class handles {
        memory::unique_alloc<SYSTEM_HANDLE_INFORMATION_EX> _handle_info;
        unsigned long                                      _buffer_size = 0;

    public:
        using value_type = handle_info;
        using iterator   = handle_info*;

        NT_FN update() noexcept
        {
            void*                        buffer;
            unsigned long                size;
            SYSTEM_HANDLE_INFORMATION_EX temp_handle_info;

            if(_buffer_size) {
                size   = _buffer_size;
                buffer = _handle_info.get();
            }
            else {
                size   = sizeof(temp_handle_info);
                buffer = &temp_handle_info;
            }

            const auto ntqsi = LI_NT(NtQuerySystemInformation);

            auto status = ntqsi(SystemExtendedHandleInformation, buffer, size, &size);

            if(status == STATUS_INFO_LENGTH_MISMATCH) {
                // add an extra page for more information
                size += 0x1000;

                _buffer_size = 0;
                ret_on_err(_handle_info.allocate(size, PAGE_READWRITE));

                // set the buffer size
                _buffer_size = size;

                status = ntqsi(
                    SystemExtendedHandleInformation, _handle_info.get(), size, &size);
            }

            return status;
        }

        NTW_INLINE explicit operator bool() const noexcept { return !!_handle_info; }

        NTW_INLINE iterator begin() noexcept
        {
            return reinterpret_cast<iterator>(_handle_info->Handles);
        }

        NTW_INLINE iterator end() noexcept
        {
            return reinterpret_cast<iterator>(_handle_info->Handles +
                                              _handle_info->NumberOfHandles);
        }
    };

} // namespace ntw::system
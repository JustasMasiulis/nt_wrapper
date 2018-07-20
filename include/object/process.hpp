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
#include "handle.hpp"
#include "../util.hpp"

namespace ntw::obj {

    namespace detail {

        template<class Handle>
        class basic_process {
            Handle _handle{ NtCurrentProcess() };

        public:
            basic_process() = default;

            template<class ObjectHandle>
            NTW_INLINE basic_process(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            template<class PidT>
            NT_FN open(PidT pid, ACCESS_MASK access) noexcept;

            template<class Buffer, NTW_QUERY_BUFFER_REQUIREMENT>
            NT_FN info(PROCESSINFOCLASS info_class,
                       Buffer&          buffer,
                       unsigned long    size     = sizeof(Buffer),
                       unsigned long*   returned = nullptr) const noexcept;

            template<class Callback, class... Args, NTW_QUERY_CALLBACK_REQUIREMENT>
            NT_FN info(PROCESSINFOCLASS info_class, Callback cb, Args&&... args) const
                noexcept;

            template<class Buffer>
            NT_FN set_info(PROCESSINFOCLASS info_class,
                           Buffer&          buffer,
                           unsigned long    info_size) const noexcept;

            template<class IdT>
            NT_FN id(IdT& id);

            NT_FN terminate(NTSTATUS exit_status) const noexcept;

            NT_FN suspend() const noexcept;

            NT_FN resume() const noexcept;

            NT_FN peb_address(std::uint64_t& address, bool same_arch) const noexcept;

            template<class Callback>
            NT_FN enum_modules(Callback cb) const noexcept;
        };

        struct enum_modules_t;

    } // namespace detail

    using unique_process = detail::basic_process<unique_handle>;
    using process_ref    = detail::basic_process<handle_ref>;

    class process_module_info {
        std::uint64_t _address;
        std::uint32_t _size;
        std::uint16_t _path_len;
        wchar_t       _full_path[MAX_PATH];

        friend detail::enum_modules_t;

    public:
        NTW_INLINE process_module_info() noexcept = default;

        NTW_INLINE std::uint64_t base_address() const noexcept;
        NTW_INLINE std::uint32_t size() const noexcept;

        NTW_INLINE std::wstring_view name() const noexcept;

        NTW_INLINE std::wstring_view full_path() const noexcept;

        NTW_INLINE const wchar_t* path_data() const noexcept;
        NTW_INLINE wchar_t* path_data() noexcept;
        NTW_INLINE std::uint16_t path_length() const noexcept;
    };

} // namespace ntw::obj

#include "../impl/process.inl"

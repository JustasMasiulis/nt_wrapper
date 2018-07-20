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
#include "event.hpp"

namespace ntw::obj {

    namespace detail {

        template<class Handle>
        struct basic_registry {
            Handle _handle;

        public:
            NTW_INLINE basic_registry() noexcept = default;
            NTW_INLINE basic_registry(void* handle) noexcept : _handle(handle) {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            template<class StringRef>
            NT_FN create_key(const StringRef& path,
                             ACCESS_MASK      access      = KEY_ALL_ACCESS,
                             bool             is_volatile = true)
            {
                auto  upath = make_ustr(path);
                auto  attr  = make_attributes(&path, OBJ_CASE_INSENSITIVE);
                ULONG disposition;
                return LI_NT(NtCreateKey)(_handle.addressof(),
                                          access,
                                          &attr,
                                          0,
                                          nullptr,
                                          is_volatile ? REG_OPTION_VOLATILE : 0,
                                          &disposition);
            }

            template<class StringRef>
            NT_FN open_key(const StringRef& path, ACCESS_MASK access = KEY_ALL_ACCESS)
            {
                auto upath = make_ustr(path);
                auto attr  = make_attributes(&upath, OBJ_CASE_INSENSITIVE);
                return LI_NT(NtOpenKeyEx)(_handle.addressof(), access, &attr, 0);
            }

            template<class StringRef>
            NT_FN set_value_key(const StringRef& path,
                                unsigned long    type,
                                void*            data,
                                unsigned long    size) const
            {
                auto upath = make_ustr(path);
                return LI_NT(NtSetValueKey)(_handle.get(), &upath, 0, type, data, size);
            }

            template<class StringRef>
            NT_FN set_value_key(const StringRef& path, unsigned long data) const
            {
                return set_value_key(path, REG_DWORD, &data, sizeof(unsigned long));
            }

            NT_FN delete_key() const { return LI_NT(NtDeleteKey)(_handle.get()); }

            template<class EventHandle>
            NT_FN notify_change(const EventHandle& event,
                                unsigned long      filter,
                                bool               watch_subkeys = false)
            {
                IO_STATUS_BLOCK iosb;
                return LI_NT(NtNotifyChangeKey)(_handle.get(),
                                                unwrap_handle(event),
                                                nullptr,
                                                nullptr,
                                                &iosb,
                                                filter,
                                                watch_subkeys,
                                                nullptr,
                                                0,
                                                true);
            }
        };

    } // namespace detail

    using unique_registry = detail::basic_registry<unique_handle>;
    using registry_ref    = detail::basic_registry<handle_ref>;

} // namespace ntw::obj

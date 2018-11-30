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
#include "../util.hpp"

namespace ntw {

	namespace detail {
    

	
	}

    template<class Handle>
    struct basic_registry_key {
        Handle _handle;

        template<class Fn, class... Tx>
        NT_FN
        _create(Fn f, UNICODE_STRING path, ACCESS_MASK access, ulong_t options, Tx... t)
        {
            auto attr = make_attributes(&path, OBJ_CASE_INSENSITIVE);

            void*      temp_handle = nullptr;
            const auto status =
                f(&temp_handle, access, &attr, 0, nullptr, options, t..., nullptr);

            if(NT_SUCCESS(status))
                _handle.reset(temp_handle);

            return status;
        }

        template<class Fn, class... Tx>
        NT_FN _open(Fn f, UNICODE_STRING path, ACCESS_MASK access, Tx... tx)
        {
            auto attr = make_attributes(&path, OBJ_CASE_INSENSITIVE);

            void*      temp_handle = nullptr;
            const auto status      = f(&temp_handle, access, &attr, 0, tx...);

            if(NT_SUCCESS(status))
                _handle.reset(temp_handle);

            return status;
        }

    public:
        NTW_INLINE basic_registry_key() noexcept = default;

        template<class ObjectHandle>
        NTW_INLINE basic_registry_key(const ObjectHandle& handle)
            : _handle(unwrap_handle(handle))
        {}

        NTW_INLINE Handle& handle() noexcept { return _handle; }
        NTW_INLINE const Handle& handle() const noexcept { return _handle; }

        template<class StringRef>
        NT_FN create(const StringRef& path,
                     ACCESS_MASK      access  = KEY_ALL_ACCESS,
                     ulong_t          options = 0)
        {
            return _create(LI_NT(NtCreateKey), make_ustr(path), access, options);
        }

        template<class TxHandle, class StringRef>
        NT_FN create_transacted(const TxHandle&  transaction,
                                const StringRef& path,
                                ACCESS_MASK      access  = KEY_ALL_ACCESS,
                                ulong_t          options = 0)
        {
            return _create(LI_NT(NtCreateKeyTransacted),
                           make_ustr(path),
                           access,
                           options,
                           unwrap_handle(transaction));
        }

        template<class StringRef>
        NT_FN open(const StringRef& path, ACCESS_MASK access = KEY_ALL_ACCESS)
        {
            _open(LI_NT(NtOpenKeyEx), make_ustr(path), access);
        }

        template<class TxHandle, class StringRef>
        NT_FN open_transacted(const TxHandle&  transaction,
                              const StringRef& path,
                              ACCESS_MASK      access = KEY_ALL_ACCESS)
        {
            _open(LI_NT(NtOpenKeyTransactedEx),
                  make_ustr(path),
                  access,
                  unwrap_handle(transaction));
        }

        template<class StringRef>
        NT_FN set(const StringRef& path,
                  unsigned long    type,
                  void*            data,
                  unsigned long    size) const
        {
            auto upath = make_ustr(path);
            return LI_NT(NtSetValueKey)(_handle.get(), &upath, 0, type, data, size);
        }

        template<class StringRef>
        NT_FN set(const StringRef& path, ulong_t data) const
        {
            return set_value(path, REG_DWORD, &data, sizeof(ulong_t));
        }

        NT_FN destroy() const { return LI_NT(NtDeleteKey)(_handle.get()); }

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

} // namespace ntw
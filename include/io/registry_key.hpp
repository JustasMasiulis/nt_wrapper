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
#include "../object/handle.hpp"
#include "../byte_span.hpp"

namespace ntw::io {

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
            return _open(LI_NT(NtOpenKeyEx), make_ustr(path), access);
        }

        template<class TxHandle, class StringRef>
        NT_FN open_transacted(const TxHandle&  transaction,
                              const StringRef& path,
                              ACCESS_MASK      access = KEY_ALL_ACCESS)
        {
            return _open(LI_NT(NtOpenKeyTransactedEx),
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

        template<class StringRef, class T>
        NT_FN get(const StringRef& path, T& buffer) const
        {
            std::aligned_storage_t<12 + sizeof(T)> storage;

            auto       upath = make_ustr(path);
            ULONG      retlen;
            const auto status = LI_NT(NtQueryValueKey)(_handle.get(),
                                                       &upath,
                                                       KeyValuePartialInformation,
                                                       &storage,
                                                       sizeof(storage),
                                                       &retlen);

            // may be unaligned? pretty bad but :shrug:
            if(NT_SUCCESS(status))
                buffer = *reinterpret_cast<T*>(reinterpret_cast<char*>(&storage) + 12);

            return status;
        }

        template<class StringRef, class Callback, class... Args>
        NT_FN get(const StringRef& path, Callback cb, Args&&... args) const
        {
            memory::unique_alloc<KEY_VALUE_PARTIAL_INFORMATION_ALIGN64> info;

            auto  upath  = make_ustr(path);
            ULONG size   = 0;
            auto  status = LI_NT(NtQueryValueKey)(_handle.get(),
                                                 &upath,
                                                 KeyValuePartialInformationAlign64,
                                                 info.get(),
                                                 size,
                                                 &size);
            if(status != STATUS_BUFFER_OVERFLOW)
                return status;

            ret_on_err(info.allocate(size, PAGE_READWRITE));

            status = LI_NT(NtQueryValueKey)(_handle.get(),
                                            &upath,
                                            KeyValuePartialInformationAlign64,
                                            info.get(),
                                            size,
                                            &size);

            if(NT_SUCCESS(status))
                cb(*info, std::forward<Args>(args)...);

            return status;
        }

        template<class StringRef>
        NT_FN set(const StringRef& path, ulong_t data) const
        {
            return set_value(path, REG_DWORD, &data, sizeof(ulong_t));
        }

        NT_FN destroy() const { return LI_NT(NtDeleteKey)(_handle.get()); }

        NT_FN subkey(KEY_INFORMATION_CLASS info_class,
                     byte_span<ulong_t>    buffer,
                     ulong_t               index)
        {
            ulong_t result_size = 0;
            return LI_NT(NtEnumerateKey)(_handle.get(),
                                         index,
                                         info_class,
                                         buffer.begin(),
                                         buffer.size(),
                                         &result_size);
        }

        template<class KeyInfoType,
                 std::size_t ExtraSize = 0,
                 class Callback,
                 class... Args>
        NT_FN enum_subkeys(KEY_INFORMATION_CLASS info_class, Callback cb, Args&&... args)
        {
            std::aligned_storage_t<sizeof(KeyInfoType) + ExtraSize> storage;
            for(ulong_t i = 0;; ++i) {
                const auto status = subkey(info_class, { &storage, &storage + 1 }, i);
                if(!NT_SUCCESS(status))
                    return status;

                NTW_CALLBACK_BREAK_IF_FALSE(
                    cb, *reinterpret_cast<KeyInfoType*>(&storage), args...);
            }

            return STATUS_SUCCESS;
        }

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

    using unique_registry_key = basic_registry_key<unique_handle>;
    using registry_key_ref    = basic_registry_key<handle_ref>;


} // namespace ntw::io
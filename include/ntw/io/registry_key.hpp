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
#include "../ob/object.hpp"
#include "../access.hpp"

namespace ntw::io {

    /// \brief Extends access_builder to contain all registry specific access flags.
    struct reg_access : access_builder<reg_access> {
        /// \note Corresponds to KEY_CREATE_LINK flag.
        NTW_INLINE constexpr reg_access& create_link() noexcept;

        /// \note Corresponds to KEY_CREATE_SUB_KEY flag.
        NTW_INLINE constexpr reg_access& create_sub_key() noexcept;

        /// \note Corresponds to KEY_ENUMERATE_SUB_KEYS flag.
        NTW_INLINE constexpr reg_access& enum_sub_keys() noexcept;

        /// \note Corresponds to KEY_NOTIFY flag.
        NTW_INLINE constexpr reg_access& notify() noexcept;

        /// \note Corresponds to KEY_QUERY_VALUE flag.
        NTW_INLINE constexpr reg_access& query_value() noexcept;

        /// \note Corresponds to KEY_SET_VALUE flag.
        NTW_INLINE constexpr reg_access& set_value() noexcept;

        /// \note Corresponds to KEY_READ flag.
        NTW_INLINE constexpr reg_access& read() noexcept;

        /// \note Corresponds to KEY_WRITE flag.
        NTW_INLINE constexpr reg_access& write() noexcept;

        /// \note Corresponds to KEY_EXECUTE flag.
        NTW_INLINE constexpr reg_access& execute() noexcept;

        /// \note Corresponds to KEY_WOW64_32KEY flag.
        NTW_INLINE constexpr reg_access& wow64_32() noexcept;

        /// \note Corresponds to KEY_WOW64_64KEY flag.
        NTW_INLINE constexpr reg_access& wow64_64() noexcept;

        /// \note Corresponds to KEY_ALL_ACCESS flag.
        NTW_INLINE constexpr reg_access& all() noexcept;
    };

    struct reg_open_options {
        NTW_INLINE constexpr reg_open_options() = default;

        /// \note Corresponds to REG_OPTION_BACKUP_RESTORE flag.
        NTW_INLINE constexpr reg_open_options& backup_restore() noexcept;

        /// \note Corresponds to REG_OPTION_OPEN_LINK flag.
        NTW_INLINE constexpr reg_open_options& open_link() noexcept;

        /// \note Corresponds to REG_OPTION_DONT_VIRTUALIZE flag.
        NTW_INLINE constexpr reg_open_options& dont_virtualize() noexcept;

        /// \brief Returns stored flags
        NTW_INLINE constexpr ntw::ulong_t get() const noexcept;

    private:
        ntw::ulong_t _value = 0;
    };

    struct reg_create_options {
        NTW_INLINE constexpr reg_create_options() = default;

        /// \note Corresponds to REG_OPTION_BACKUP_RESTORE flag.
        NTW_INLINE constexpr reg_create_options& backup_restore() noexcept;

        /// \note Corresponds to REG_OPTION_OPEN_LINK flag.
        NTW_INLINE constexpr reg_create_options& open_link() noexcept;

        /// \note Corresponds to REG_OPTION_DONT_VIRTUALIZE flag.
        NTW_INLINE constexpr reg_create_options& dont_virtualize() noexcept;

        /// \note Corresponds to REG_OPTION_VOLATILE flag.
        NTW_INLINE constexpr reg_create_options& non_preserved() noexcept;

        /// \note Corresponds to REG_OPTION_CREATE_LINK flag.
        NTW_INLINE constexpr reg_create_options& create_link() noexcept;

        /// \brief Returns stored flags
        NTW_INLINE constexpr ntw::ulong_t get() const noexcept;

    private:
        ntw::ulong_t _value = 0;
    };

    template<class Handle>
    struct basic_reg_key : Handle {
        using handle_type = Handle;

        // inherit constructors
        using handle_type::handle_type;

        // TODO transacted API
        NTW_INLINE static result<basic_reg_key> create(
            unicode_string     path,
            reg_access         access,
            reg_create_options options    = {},
            ob::attributes     attributes = {}) noexcept;

        NTW_INLINE static result<basic_reg_key> create(unicode_string     path,
                                                       reg_access         access,
                                                       reg_create_options options,
                                                       ob::attributes     attributes,
                                                       bool& opened_existing) noexcept;

        NTW_INLINE static result<basic_reg_key> open(
            unicode_string   path,
            reg_access       access,
            reg_open_options options,
            ob::attributes   attributes = {}) noexcept;

        NTW_INLINE static result<basic_reg_key> open(
            unicode_string path,
            reg_access     access,
            ob::attributes attributes = {}) noexcept;

        // TODO better buffer alternatives
        NTW_INLINE status set(unicode_string name,
                              unsigned long  type,
                              void*          data,
                              unsigned long  size) const;

        NTW_INLINE status set(unicode_string name, ntw::ulong_t value) const;

        /*template<class T>
        NT_FN _get(UNICODE_STRING path, T& x) const
        {
            std::aligned_storage_t<12 + sizeof(T)> storage;

            ULONG      retlen;
            const auto status = NTW_SYSCALL(NtQueryValueKey)(_handle.get(),
                                                             &path,
                                                             KeyValuePartialInformation,
                                                             &storage,
                                                             sizeof(storage),
                                                             &retlen);

            if(NT_SUCCESS(status))
                x = *reinterpret_cast<T*>(reinterpret_cast<char*>(&storage) + 12);

            return status;
        }

        NT_FN get(unicode_string name,
                  unsigned long  type,
                  void*          data,
                  unsigned long  size) const
        {
            return NTW_SYSCALL(NtQueryValueKey)(get(), &name.get(), 0, type, data, size);
        }


        template<class StringRef>
        NT_FN query_value(const StringRef&            path,
                          KEY_VALUE_INFORMATION_CLASS info_class,
                          byte_span<ulong_t>          buffer) const
        {
            auto  upath = make_ustr(path);
            ULONG retlen;
            return NTW_SYSCALL(NtQueryValueKey)(_handle.get(),
                                                &upath,
                                                info_class,
                                                buffer.begin(),
                                                buffer.size(),
                                                &retlen);
        }

        template<class StringRef>
        NT_FN get(const StringRef& path, std::uint32_t& dword) const
        {
            return _get(make_ustr(path), dword);
        }

        template<class StringRef>
        NT_FN get(const StringRef& path, std::uint64_t& qword) const
        {
            return _get(make_ustr(path), qword);
        }

        template<std::size_t MaxSize, class StringRef, class Callback, class... Args>
        NT_FN get(const StringRef& path, Callback cb, Args&&... args) const
        {
            std::aligned_storage_t<12 + MaxSize> storage;

            auto       upath  = make_ustr(path);
            ULONG      size   = 0;
            const auto status = NTW_SYSCALL(NtQueryValueKey)(_handle.get(),
                                                             &upath,
                                                             KeyValuePartialInformation,
                                                             &storage,
                                                             sizeof(storage),
                                                             &size);
            if(NT_SUCCESS(status)) {
                const auto info =
                    reinterpret_cast<KEY_VALUE_PARTIAL_INFORMATION*>(&storage);
                cb(static_cast<void*>(info->Data),
                   info->DataLength,
                   std::forward<Args>(args)...);
            }

            return status;
        }

        template<class StringRef, class Callback, class... Args>
        NT_FN get(const StringRef& path, Callback cb, Args&&... args) const
        {
            memory::unique_alloc<KEY_VALUE_PARTIAL_INFORMATION_ALIGN64> info;

            auto  upath  = make_ustr(path);
            ULONG size   = 0;
            auto  status = NTW_SYSCALL(NtQueryValueKey)(_handle.get(),
                                                       &upath,
                                                       KeyValuePartialInformationAlign64,
                                                       info.get(),
                                                       size,
                                                       &size);
            if(!NT_SUCCESS(status) && status != STATUS_BUFFER_TOO_SMALL)
                return status;

            ret_on_err(info.allocate(size, PAGE_READWRITE));

            status = NTW_SYSCALL(NtQueryValueKey)(_handle.get(),
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

        NT_FN destroy() const { return NTW_SYSCALL(NtDeleteKey)(_handle.get()); }

        NT_FN subkey(KEY_INFORMATION_CLASS info_class,
                     byte_span<ulong_t>    buffer,
                     ulong_t               index)
        {
            ulong_t result_size = 0;
            return NTW_SYSCALL(NtEnumerateKey)(_handle.get(),
                                               index,
                                               info_class,
                                               buffer.begin(),
                                               buffer.size(),
                                               &result_size);
        }

        template<class KeyInfoType, std::size_t ExtraSize, class Callback, class... Args>
        NT_FN enum_subkeys(KEY_INFORMATION_CLASS info_class, Callback cb, Args&&... args)
        {
            std::aligned_storage_t<sizeof(KeyInfoType) + ExtraSize> storage;
            for(ulong_t i = 0;; ++i) {
                const auto status = subkey(info_class, { &storage, &storage + 1 }, i);
                if(!NT_SUCCESS(status))
                    if(status == STATUS_NO_MORE_ENTRIES)
                        return STATUS_SUCCESS;
                    else
                        return status;

                NTW_CALLBACK_BREAK_IF_FALSE(
                    cb, *reinterpret_cast<KeyInfoType*>(&storage), args...);
            }

            return STATUS_SUCCESS;
        }

        template<class KeyInfoType, std::size_t ExtraSize, class Callback, class... Args>
        NT_FN enum_values(KEY_VALUE_INFORMATION_CLASS info_class,
                          Callback                    cb,
                          Args&&... args)
        {
            std::aligned_storage_t<sizeof(KeyInfoType) + ExtraSize> storage;
            ulong_t                                                 retlen;
            for(ulong_t i = 0;; ++i) {
                const auto status = NTW_SYSCALL(NtEnumerateValueKey)(
                    _handle.get(), i, info_class, &storage, sizeof(storage), &retlen);

                if(!NT_SUCCESS(status))
                    if(status == STATUS_NO_MORE_ENTRIES)
                        return STATUS_SUCCESS;
                    else
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
            return NTW_SYSCALL(NtNotifyChangeKey)(_handle.get(),
                                                  unwrap_handle(event),
                                                  nullptr,
                                                  nullptr,
                                                  &iosb,
                                                  filter,
                                                  watch_subkeys,
                                                  nullptr,
                                                  0,
                                                  true);
        }*/
    };

    using unique_reg_key = basic_reg_key<ntw::ob::object>;
    using reg_key_ref    = basic_reg_key<ntw::ob::object_ref>;

} // namespace ntw::io

#include "../../../impl/io/registry_key.inl"
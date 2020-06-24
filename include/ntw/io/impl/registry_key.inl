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
#include "../registry_key.hpp"

namespace ntw::io {

    NTW_INLINE constexpr reg_access& reg_access::create_link() noexcept
    {
        _access |= KEY_CREATE_LINK;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::create_sub_key() noexcept
    {
        _access |= KEY_CREATE_SUB_KEY;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::enum_sub_keys() noexcept
    {
        _access |= KEY_ENUMERATE_SUB_KEYS;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::notify() noexcept
    {
        _access |= KEY_NOTIFY;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::query_value() noexcept
    {
        _access |= KEY_QUERY_VALUE;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::set_value() noexcept
    {
        _access |= KEY_SET_VALUE;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::read() noexcept
    {
        _access |= KEY_READ;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::write() noexcept
    {
        _access |= KEY_WRITE;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::execute() noexcept
    {
        _access |= KEY_EXECUTE;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::wow64_32() noexcept
    {
        _access |= KEY_WOW64_32KEY;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::wow64_64() noexcept
    {
        _access |= KEY_WOW64_64KEY;
        return *this;
    }

    NTW_INLINE constexpr reg_access& reg_access::all() noexcept
    {
        _access |= KEY_ALL_ACCESS;
        return *this;
    }

    // reg_open_options -------------------------------------------------------

    NTW_INLINE constexpr reg_open_options& reg_open_options::backup_restore() noexcept
    {
        _value |= REG_OPTION_BACKUP_RESTORE;
        return *this;
    }

    NTW_INLINE constexpr reg_open_options& reg_open_options::open_link() noexcept
    {
        _value |= REG_OPTION_OPEN_LINK;
        return *this;
    }

    NTW_INLINE constexpr reg_open_options& reg_open_options::dont_virtualize() noexcept
    {
        _value |= REG_OPTION_DONT_VIRTUALIZE;
        return *this;
    }

    NTW_INLINE constexpr ntw::ulong_t reg_open_options::get() const noexcept
    {
        return _value;
    }

    // reg_create_options -----------------------------------------------------

    NTW_INLINE constexpr reg_create_options& reg_create_options::backup_restore() noexcept
    {
        _value |= REG_OPTION_BACKUP_RESTORE;
        return *this;
    }

    NTW_INLINE constexpr reg_create_options& reg_create_options::open_link() noexcept
    {
        _value |= REG_OPTION_OPEN_LINK;
        return *this;
    }

    NTW_INLINE constexpr reg_create_options&
    reg_create_options::dont_virtualize() noexcept
    {
        _value |= REG_OPTION_DONT_VIRTUALIZE;
        return *this;
    }

    NTW_INLINE constexpr reg_create_options& reg_create_options::non_preserved() noexcept
    {
        _value |= REG_OPTION_VOLATILE;
        return *this;
    }

    NTW_INLINE constexpr reg_create_options& reg_create_options::create_link() noexcept
    {
        _value |= REG_OPTION_CREATE_LINK;
        return *this;
    }

    NTW_INLINE constexpr ntw::ulong_t reg_create_options::get() const noexcept
    {
        return _value;
    }

    // basic_reg_key ----------------------------------------------------------

    template<class H>
    NTW_INLINE result<basic_reg_key<H>>
               basic_reg_key<H>::create(unicode_string     path,
                             reg_access         access,
                             reg_create_options options,
                             ob::attributes     attributes) noexcept
    {
        void* handle        = nullptr;
        auto& raw_attr      = attributes.get();
        raw_attr.ObjectName = &path.get();

        const auto status = NTW_SYSCALL(NtCreateKey)(
            &handle, access.get(), &raw_attr, 0, nullptr, options.get(), nullptr);

        return { status, basic_reg_key{ handle } };
    }

    template<class H>
    NTW_INLINE result<basic_reg_key<H>>
               basic_reg_key<H>::create(unicode_string     path,
                             reg_access         access,
                             reg_create_options options,
                             ob::attributes     attributes,
                             bool&              opened_existing) noexcept
    {
        void* handle        = nullptr;
        auto& raw_attr      = attributes.get();
        raw_attr.ObjectName = &path.get();

        ulong_t    open_state;
        const auto status = NTW_SYSCALL(NtCreateKey)(
            &handle, access.get(), &raw_attr, 0, nullptr, options.get(), &open_state);

        if(NT_SUCCESS(status))
            opened_existing = open_state - 1;

        return { status, basic_reg_key{ handle } };
    }

    template<class H>
    NTW_INLINE result<basic_reg_key<H>>
               basic_reg_key<H>::open(unicode_string   path,
                           reg_access       access,
                           reg_open_options options,
                           ob::attributes   attributes) noexcept
    {
        void* handle        = nullptr;
        auto& raw_attr      = attributes.get();
        raw_attr.ObjectName = &path.get();

        const auto status =
            NTW_SYSCALL(NtOpenKeyEx)(&handle, access.get(), &raw_attr, options.get());

        return { status, basic_reg_key{ handle } };
    }

    template<class H>
    NTW_INLINE result<basic_reg_key<H>> basic_reg_key<H>::open(
        unicode_string path, reg_access access, ob::attributes attributes) noexcept
    {
        void* handle        = nullptr;
        auto& raw_attr      = attributes.get();
        raw_attr.ObjectName = &path.get();

        const auto status = NTW_SYSCALL(NtOpenKey)(&handle, access.get(), &raw_attr);

        return { status, basic_reg_key{ handle } };
    }

    template<class H>
    NTW_INLINE status basic_reg_key<H>::set(unicode_string name,
                                            unsigned long  type,
                                            void*          data,
                                            unsigned long  size) const
    {
        return NTW_SYSCALL(NtSetValueKey)(this->get(), &name.get(), 0, type, data, size);
    }

    template<class H>
    NTW_INLINE status basic_reg_key<H>::set(unicode_string name, ntw::ulong_t value) const
    {
        return NTW_SYSCALL(NtSetValueKey)(
            this->get(), &name.get(), 0, REG_DWORD, &value, 4);
    }

} // namespace ntw::io
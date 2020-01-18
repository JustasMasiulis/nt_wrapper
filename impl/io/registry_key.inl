#pragma once
#include "../../include/ntw/io/registry_key.hpp"

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

} // namespace ntw::io
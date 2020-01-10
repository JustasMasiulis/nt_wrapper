#pragma once
#include "../../include/ntw/ob/token.hpp"

namespace ntw::ob {

    NTW_INLINE constexpr token_access& token_access::adjust_default() noexcept
    {
        _access |= TOKEN_ADJUST_DEFAULT;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::adjust_groups() noexcept
    {
        _access |= TOKEN_ADJUST_GROUPS;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::adjust_privileges() noexcept
    {
        _access |= TOKEN_ADJUST_PRIVILEGES;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::adjust_sessionid() noexcept
    {
        _access |= TOKEN_ADJUST_SESSIONID;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::assign_primary() noexcept
    {
        _access |= TOKEN_ASSIGN_PRIMARY;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::duplicate() noexcept
    {
        _access |= TOKEN_DUPLICATE;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::execute() noexcept
    {
        _access |= TOKEN_EXECUTE;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::impersonate() noexcept
    {
        _access |= TOKEN_IMPERSONATE;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::query() noexcept
    {
        _access |= TOKEN_QUERY;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::query_source() noexcept
    {
        _access |= TOKEN_QUERY_SOURCE;
        return *this;
    }

    NTW_INLINE constexpr token_access& token_access::all() noexcept
    {
        _access |= TOKEN_ALL_ACCESS;
        return *this;
    }

    template<class H>
    template<class HP>
    NTW_INLINE ntw::result<basic_token<H>> basic_token<H>::open(
        const basic_process<HP>& process, token_access desired_access) noexcept
    {
        void*      handle = nullptr;
        const auto status =
            NTW_SYSCALL(NtOpenProcessToken)(process.get(), desired_access.get(), &handle);
        return { status, basic_token<H>{ handle } };
    }

} // namespace ntw::ob
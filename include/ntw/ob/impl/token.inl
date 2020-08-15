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
#include "../token.hpp"

namespace ntw::ob {

    NTW_INLINE constexpr privilege_with_attributes privilege::enable() const noexcept
    {
        return privilege_with_attributes{ *this, SE_PRIVILEGE_ENABLED };
    }

    NTW_INLINE constexpr privilege_with_attributes privilege::disable() const noexcept
    {
        return privilege_with_attributes{ *this, 0 };
    }

    NTW_INLINE constexpr privilege_with_attributes privilege::remove() const noexcept
    {
        return privilege_with_attributes{ *this, SE_PRIVILEGE_REMOVED };
    }

    NTW_INLINE constexpr privilege privilege::create_token() noexcept
    {
        return { SE_CREATE_TOKEN_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::assign_primary_token() noexcept
    {
        return { SE_ASSIGNPRIMARYTOKEN_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::lock_memory() noexcept
    {
        return { SE_LOCK_MEMORY_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::increase_qouta() noexcept
    {
        return { SE_INCREASE_QUOTA_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::machine_account() noexcept
    {
        return { SE_MACHINE_ACCOUNT_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::tcb() noexcept
    {
        return { SE_TCB_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::security() noexcept
    {
        return { SE_SECURITY_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::take_ownership() noexcept
    {
        return { SE_TAKE_OWNERSHIP_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::load_driver() noexcept
    {
        return { SE_LOAD_DRIVER_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::system_profile() noexcept
    {
        return { SE_SYSTEM_PROFILE_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::systemtime() noexcept
    {
        return { SE_SYSTEMTIME_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::prof_single_process() noexcept
    {
        return { SE_PROF_SINGLE_PROCESS_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::inc_base_priority() noexcept
    {
        return { SE_INC_BASE_PRIORITY_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::create_pagefile() noexcept
    {
        return { SE_CREATE_PAGEFILE_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::create_permanent() noexcept
    {
        return { SE_CREATE_PERMANENT_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::backup() noexcept
    {
        return { SE_BACKUP_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::restore() noexcept
    {
        return { SE_RESTORE_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::shutdown() noexcept
    {
        return { SE_SHUTDOWN_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::debug() noexcept
    {
        return { SE_DEBUG_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::audit() noexcept
    {
        return { SE_AUDIT_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::system_environment() noexcept
    {
        return { SE_SYSTEM_ENVIRONMENT_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::change_notify() noexcept
    {
        return { SE_CHANGE_NOTIFY_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::remote_shutdown() noexcept
    {
        return { SE_REMOTE_SHUTDOWN_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::undock() noexcept
    {
        return { SE_UNDOCK_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::sync_agent() noexcept
    {
        return { SE_SYNC_AGENT_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::enable_delegation() noexcept
    {
        return { SE_ENABLE_DELEGATION_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::manage_volume() noexcept
    {
        return { SE_MANAGE_VOLUME_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::impersonate() noexcept
    {
        return { SE_IMPERSONATE_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::create_global() noexcept
    {
        return { SE_CREATE_GLOBAL_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::trusted_credman_access() noexcept
    {
        return { SE_TRUSTED_CREDMAN_ACCESS_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::relabel() noexcept
    {
        return { SE_RELABEL_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::inc_working_set() noexcept
    {
        return { SE_INC_WORKING_SET_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::time_zone() noexcept
    {
        return { SE_TIME_ZONE_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::create_symbolic_link() noexcept
    {
        return { SE_CREATE_SYMBOLIC_LINK_PRIVILEGE };
    }

    NTW_INLINE constexpr privilege privilege::delegate_session_user_impersonate() noexcept
    {
        return { SE_DELEGATE_SESSION_USER_IMPERSONATE_PRIVILEGE };
    }

    // privilege_with_attributes ----------------------------------------------

    NTW_INLINE constexpr privilege_with_attributes&
    privilege_with_attributes::enable() noexcept
    {
        attributes = SE_PRIVILEGE_ENABLED;
        return *this;
    }

    NTW_INLINE constexpr privilege_with_attributes&
    privilege_with_attributes::remove() noexcept
    {
        attributes = SE_PRIVILEGE_REMOVED;
        return *this;
    }

    NTW_INLINE constexpr bool privilege_with_attributes::enabled() const noexcept
    {
        return attributes & SE_PRIVILEGE_ENABLED;
    }

    NTW_INLINE bool privilege_with_attributes::removed() const noexcept
    {
        return attributes & SE_PRIVILEGE_REMOVED;
    }

    NTW_INLINE bool privilege_with_attributes::enabled_by_default() const noexcept
    {
        return attributes & SE_PRIVILEGE_ENABLED_BY_DEFAULT;
    }

    // token_access -----------------------------------------------------------

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

    template<class H>
    template<class HT>
    NTW_INLINE ntw::result<basic_token<H>> basic_token<H>::open(
        const basic_thread<HT>& thread, token_access desired_access) noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtOpenThreadToken)(
            thread->get(), desired_access.get(), false, &handle);
        return { status, { handle } };
    }

    template<class H>
    template<class HT>
    NTW_INLINE ntw::result<basic_token<H>> basic_token<H>::open_as_self(
        const basic_thread<HT>& thread, token_access desired_access) noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtOpenThreadToken)(
            thread->get(), desired_access.get(), true, &handle);
        return { status, { handle } };
    }

    template<class H>
    NTW_INLINE status basic_token<H>::reset_privileges() const noexcept
    {
        return NTW_SYSCALL(NtAdjustPrivilegesToken)(
            this->get(), true, nullptr, 0, nullptr, nullptr);
    }

    template<class H>
    NTW_INLINE result<privilege_with_attributes>
               basic_token<H>::adjust_privilege(privilege_with_attributes privilege) const noexcept
    {
        struct {
            std::uint32_t             count;
            privilege_with_attributes priv;
        } state{ 1, privilege };

        ntw::ulong_t ret_size = sizeof(state);
        const auto   status   = NTW_SYSCALL(NtAdjustPrivilegesToken)(
            this->get(),
            FALSE,
            reinterpret_cast<TOKEN_PRIVILEGES*>(&state),
            unsigned{ sizeof(state) },
            reinterpret_cast<TOKEN_PRIVILEGES*>(&state),
            &ret_size);

        return { status, state.priv };
    }

    template<class H>
    NTW_INLINE status
    basic_token<H>::replace_privilege(privilege_with_attributes privilege) const noexcept
    {
        struct {
            std::uint32_t             count;
            privilege_with_attributes priv;
        } state{ 1, privilege };

        auto status = NTW_SYSCALL(NtAdjustPrivilegesToken)(
            this->get(),
            FALSE,
            reinterpret_cast<TOKEN_PRIVILEGES*>(&state),
            unsigned{ sizeof(state) },
            nullptr,
            nullptr);

        return status;
    }

} // namespace ntw::ob
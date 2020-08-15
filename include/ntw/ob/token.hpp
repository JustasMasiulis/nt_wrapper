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
#include "object.hpp"
#include "process.hpp"
#include "thread.hpp"

namespace ntw::ob {

    /// \brief Extends access_builder to contain all token specific access flags.
    struct token_access : access_builder<process_access> {
        /// \brief Required to change the default owner, primary group, or DACL of an
        /// access token. \note Corresponds to TOKEN_ADJUST_DEFAULT flag.
        NTW_INLINE constexpr token_access& adjust_default() noexcept;

        /// \brief Required to adjust the attributes of the groups in an access token.
        /// \note Corresponds to TOKEN_ADJUST_GROUPS flag.
        NTW_INLINE constexpr token_access& adjust_groups() noexcept;

        /// \brief Required to adjust the session ID of an access token. The SE_TCB_NAME
        /// privilege is required. \note Corresponds to TOKEN_ADJUST_SESSIONID flag.
        NTW_INLINE constexpr token_access& adjust_privileges() noexcept;

        /// \brief Required to adjust the session ID of an access token. The SE_TCB_NAME
        /// privilege is required. \note Corresponds to TOKEN_ADJUST_SESSIONID flag.
        NTW_INLINE constexpr token_access& adjust_sessionid() noexcept;

        /// \brief Required to attach a primary token to a process. The
        /// SE_ASSIGNPRIMARYTOKEN_NAME privilege is also required to accomplish this task.
        /// \note Corresponds to TOKEN_ASSIGN_PRIMARY flag.
        NTW_INLINE constexpr token_access& assign_primary() noexcept;

        /// \brief Required to duplicate an access token.
        /// \note Corresponds to TOKEN_DUPLICATE flag.
        NTW_INLINE constexpr token_access& duplicate() noexcept;

        /// \brief Combines STANDARD_RIGHTS_EXECUTE and TOKEN_IMPERSONATE.
        /// \note Corresponds to TOKEN_EXECUTE flag.
        NTW_INLINE constexpr token_access& execute() noexcept;

        /// \brief Required to attach an impersonation access token to a process.
        /// \note Corresponds to TOKEN_IMPERSONATE flag.
        NTW_INLINE constexpr token_access& impersonate() noexcept;

        /// \brief Required to query an access token.
        /// \note Corresponds to TOKEN_QUERY flag.
        NTW_INLINE constexpr token_access& query() noexcept;

        /// \brief Required to query the source of an access token.
        /// \note Corresponds to TOKEN_QUERY_SOURCE flag.
        NTW_INLINE constexpr token_access& query_source() noexcept;

        /// \brief Combines all possible access rights for a token.
        /// \note Corresponds to TOKEN_ALL_ACCESS flag.
        NTW_INLINE constexpr token_access& all() noexcept;
    };

    // forward declaration
    struct privilege_with_attributes;

    /// \brief Wraps LUID structure.
    struct privilege : LUID {
        /// \brief Constructs LUID with your given privilege value.
        NTW_INLINE constexpr privilege(std::uint32_t value) noexcept : LUID{ value, 0 } {}

        /// \brief Creates privilege_with_attributes and enables it.
        NTW_INLINE constexpr privilege_with_attributes enable() const noexcept;

        /// \brief Creates privilege_with_attributes with zeroed attributes.
        NTW_INLINE constexpr privilege_with_attributes disable() const noexcept;

        /// \brief Creates privilege_with_attributes and removes it.
        NTW_INLINE constexpr privilege_with_attributes remove() const noexcept;

        NTW_INLINE constexpr static privilege create_token() noexcept;

        NTW_INLINE constexpr static privilege assign_primary_token() noexcept;

        NTW_INLINE constexpr static privilege lock_memory() noexcept;

        NTW_INLINE constexpr static privilege increase_qouta() noexcept;

        NTW_INLINE constexpr static privilege machine_account() noexcept;

        NTW_INLINE constexpr static privilege tcb() noexcept;

        NTW_INLINE constexpr static privilege security() noexcept;

        NTW_INLINE constexpr static privilege take_ownership() noexcept;

        NTW_INLINE constexpr static privilege load_driver() noexcept;

        NTW_INLINE constexpr static privilege system_profile() noexcept;

        NTW_INLINE constexpr static privilege systemtime() noexcept;

        NTW_INLINE constexpr static privilege prof_single_process() noexcept;

        NTW_INLINE constexpr static privilege inc_base_priority() noexcept;

        NTW_INLINE constexpr static privilege create_pagefile() noexcept;

        NTW_INLINE constexpr static privilege create_permanent() noexcept;

        NTW_INLINE constexpr static privilege backup() noexcept;

        NTW_INLINE constexpr static privilege restore() noexcept;

        NTW_INLINE constexpr static privilege shutdown() noexcept;

        NTW_INLINE constexpr static privilege debug() noexcept;

        NTW_INLINE constexpr static privilege audit() noexcept;

        NTW_INLINE constexpr static privilege system_environment() noexcept;

        NTW_INLINE constexpr static privilege change_notify() noexcept;

        NTW_INLINE constexpr static privilege remote_shutdown() noexcept;

        NTW_INLINE constexpr static privilege undock() noexcept;

        NTW_INLINE constexpr static privilege sync_agent() noexcept;

        NTW_INLINE constexpr static privilege enable_delegation() noexcept;

        NTW_INLINE constexpr static privilege manage_volume() noexcept;

        NTW_INLINE constexpr static privilege impersonate() noexcept;

        NTW_INLINE constexpr static privilege create_global() noexcept;

        NTW_INLINE constexpr static privilege trusted_credman_access() noexcept;

        NTW_INLINE constexpr static privilege relabel() noexcept;

        NTW_INLINE constexpr static privilege inc_working_set() noexcept;

        NTW_INLINE constexpr static privilege time_zone() noexcept;

        NTW_INLINE constexpr static privilege create_symbolic_link() noexcept;

        NTW_INLINE constexpr static privilege
        delegate_session_user_impersonate() noexcept;
    };

    /// \brief LUID_AND_ATTRIBUTES reimplementation
    struct privilege_with_attributes {
        privilege     privilege;
        std::uint32_t attributes = 0;

        NTW_INLINE constexpr privilege_with_attributes& enable() noexcept;

        NTW_INLINE constexpr privilege_with_attributes& remove() noexcept;

        NTW_INLINE constexpr bool enabled() const noexcept;

        NTW_INLINE bool removed() const noexcept;

        NTW_INLINE bool enabled_by_default() const noexcept;
    };

    /// \brief Wrapper class around token object
    // TODO open overload for threads
    // TODO reset_privileges overload with old state return
    // TODO NtAdjustPrivilegesToken with array of privileges instead of singular instances
    template<class Handle>
    struct basic_token : Handle {
        using handle_type = Handle;
        using access_type = token_access;

        using handle_type::handle_type;

        basic_token() = default;

        /// \brief Opens given process token with the specified access.
        template<class H>
        NTW_INLINE static ntw::result<basic_token>
        open(const basic_process<H>& process, token_access desired_access) noexcept;

        /// \brief Opens given thread token with the specified access.
        template<class H>
        NTW_INLINE static ntw::result<basic_token>
        open(const basic_thread<H>& thread, token_access desired_access) noexcept;

        /// \brief Opens given thread token with the specified access as self.
        template<class H>
        NTW_INLINE static ntw::result<basic_token>
        open_as_self(const basic_thread<H>& thread, token_access desired_access) noexcept;

        /// \brief Resets / disables all privileges.
        /// \note Acts as AdjustTokenPrivileges(handle, TRUE, ...)
        NTW_INLINE status reset_privileges() const noexcept;

        /// \brief Ajusts a single privilege using NtAdjustPrivilegesToken API.
        /// \returns The old state of the privilege.
        /// \warn MAY RETURN STATUS_NOT_ALL_ASSIGNED WHICH IS NOT TREATED AS AN ERROR.
        /// \note If you don't care about old state use replace_privilege.
        NTW_INLINE result<privilege_with_attributes> adjust_privilege(
            privilege_with_attributes privilege) const noexcept;

        /// \brief Ajusts a single privilege using NtAdjustPrivilegesToken API.
        /// \warn MAY RETURN STATUS_NOT_ALL_ASSIGNED WHICH IS NOT TREATED AS AN ERROR.
        /// \note If you care about old state use adjust_privilege.
        NTW_INLINE status
        replace_privilege(privilege_with_attributes privilege) const noexcept;
    };

    using token     = basic_token<object>;
    using token_ref = basic_token<object_ref>;

} // namespace ntw::ob

#include "impl/token.inl"
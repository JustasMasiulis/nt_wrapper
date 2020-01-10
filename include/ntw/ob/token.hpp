#pragma once
#include "object.hpp"
#include "process.hpp"

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

    struct token_privilege_t {
        unsigned long privilege;
        bool          enable;
    };

    /// \brief Wrapper class around token object
    template<class Handle>
    struct basic_token : Handle {
        using handle_type = Handle;

        basic_token() = default;

        using handle_type::handle_type;

        /// \brief Opens given process token with the specified access.
        template<class H>
        NTW_INLINE static ntw::result<basic_token>
        open(const basic_process<H>& process, token_access desired_access) noexcept;

        // TODO
        // clang-format off
        // template<class H>
        // NTW_INLINE ntw::result<basic_token> open(const ntw::ob::basic_thread<H>& thread,
        //                                          token_access                    desired_access,
        //                                          bool                            open_as_self) noexcept
        // clang-format on
        // template<class T>
        // NT_FN info(TOKEN_INFORMATION_CLASS info_class,
        //           T&                      info,
        //           unsigned long           info_size     = sizeof(T),
        //           unsigned long*          returned_size = nullptr)
        //{
        //    unsigned long ret_size = 0;
        //    const auto    status   = LI_NT(NtQueryInformationToken)(
        //        _handle.get(), info_class, &info, info_size, &ret_size);
        //    if(returned_size)
        //        *returned_size = ret_size;
        //    return status;
        //}

        // template<std::size_t N>
        // NT_FN adjust_privileges(const token_privilege_t (&privileges)[N])
        //{
        //    struct {
        //        DWORD               PrivilegeCount = N;
        //        LUID_AND_ATTRIBUTES Privileges[N];
        //    } new_state;

        //    for(std::size_t i = 0; i < N; ++i) {
        //        auto& priv         = new_state.Privileges[i];
        //        priv.Luid.LowPart  = privileges[i].privilege;
        //        priv.Luid.HighPart = 0;
        //        priv.Attributes    = (privileges[i].enable ? SE_PRIVILEGE_ENABLED : 0);
        //    }

        //    const auto status = LI_NT(NtAdjustPrivilegesToken)(
        //        _handle.get(),
        //        FALSE,
        //        reinterpret_cast<TOKEN_PRIVILEGES*>(&new_state),
        //        unsigned{ sizeof(TOKEN_PRIVILEGES) },
        //        nullptr,
        //        nullptr);

        //    if(status == STATUS_NOT_ALL_ASSIGNED)
        //        return STATUS_PRIVILEGE_NOT_HELD;

        //    return status;
        //}

        // NT_FN adjust_privilege(unsigned long priv, bool enable)
        //{
        //    token_privilege_t p[1]{ { priv, enable } };
        //    return adjust_privileges(p);
        //}
    };

    using unique_token = basic_token<unique_object>;
    using token_ref    = basic_token<object_ref>;

} // namespace ntw::ob

#include "../impl/ob/token.inl"
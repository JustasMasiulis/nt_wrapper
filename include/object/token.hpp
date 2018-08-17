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
#include "handle.hpp"
#include "../util.hpp"

namespace ntw::obj {

    struct token_privilege_t {
        unsigned long privilege;
        bool          enable;
    };

    namespace detail {

        template<class Handle>
        class basic_token {
            Handle _handle;

        public:
            basic_token() = default;

            template<class ObjectHandle>
            NTW_INLINE basic_token(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            template<class ProcessHandle>
            NT_FN open_process(const ProcessHandle& process,
                               ACCESS_MASK          desired_access) noexcept
            {
                _handle.reset();
                return LI_NT(NtOpenProcessToken)(
                    unwrap_handle(process), desired_access, _handle.addressof());
            }

            template<class ThreadHandle>
            NT_FN open_thread(const ThreadHandle& thread,
                              ACCESS_MASK         desired_access,
                              bool                open_as_self) noexcept
            {
                _handle.reset();
                return LI_NT(NtOpenThreadToken)(
                    unwrap_handle(thread), desired_access, open_as_self);
            }

            NT_FN open_process(ACCESS_MASK desired_access)
            {
                return open_process(NtCurrentProcess(), desired_access);
            }

            NT_FN open_thread(ACCESS_MASK desired_access, bool open_as_self)
            {
                return open_process(NtCurrentThread(), open_as_self);
            }

            template<class T>
            NT_FN info(TOKEN_INFORMATION_CLASS info_class,
                       T&                      info,
                       unsigned long           info_size     = sizeof(T),
                       unsigned long*          returned_size = nullptr)
            {
                unsigned long ret_size = 0;
                const auto    status   = LI_NT(NtQueryInformationToken)(
                    _handle.get(), info_class, &info, info_size, &ret_size);
                if(returned_size)
                    *returned_size = ret_size;
                return status;
            }

            template<std::size_t N>
            NT_FN adjust_privileges(const token_privilege_t (&privileges)[N])
            {
                struct {
                    DWORD               PrivilegeCount = N;
                    LUID_AND_ATTRIBUTES Privileges[N];
                } new_state;

                for(std::size_t i = 0; i < N; ++i) {
                    auto& priv         = new_state.Privileges[i];
                    priv.Luid.LowPart  = privileges[i].privilege;
                    priv.Luid.HighPart = 0;
                    priv.Attributes = (privileges[i].enable ? SE_PRIVILEGE_ENABLED : 0);
                }

                const auto status = LI_NT(NtAdjustPrivilegesToken)(
                    _handle.get(),
                    FALSE,
                    reinterpret_cast<TOKEN_PRIVILEGES*>(&new_state),
                    unsigned{ sizeof(TOKEN_PRIVILEGES) },
                    nullptr,
                    nullptr);

                if(status == STATUS_NOT_ALL_ASSIGNED)
                    return STATUS_PRIVILEGE_NOT_HELD;

                return status;
            }

            NT_FN adjust_privilege(unsigned long priv, bool enable)
            {
                token_privilege_t p[1]{ { priv, enable } };
                return adjust_privileges(p);
            }
        };

    } // namespace detail

    using unique_token = detail::basic_token<unique_handle>;
    using token_ref    = detail::basic_token<handle_ref>;

} // namespace ntw::obj
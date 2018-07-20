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
    namespace detail {

        template<class Handle>
        class basic_thread {
            Handle _handle;

        public:
            NTW_INLINE basic_thread() noexcept = default;

            template<class ObjectHandle>
            NTW_INLINE basic_thread(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            template<class ProcessHandle>
            NT_FN
            create_remote(const ProcessHandle&       process_handle,
                          PUSER_THREAD_START_ROUTINE function_address,
                          void*                      function_arg = nullptr,
                          unsigned long   flags = THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER,
                          std::uintptr_t* thread_id = nullptr) noexcept
            {
                auto      obj_attributes = make_attributes(nullptr, OBJ_KERNEL_HANDLE);
                CLIENT_ID client_id      = {};
                PS_ATTRIBUTE_LIST ps_attributes;
                ps_attributes.TotalLength = sizeof(PS_ATTRIBUTE_LIST);
                ps_attributes.Attributes[0].Attribute =
                    ProcThreadAttributeValue(PsAttributeClientId, TRUE, FALSE, FALSE);
                ps_attributes.Attributes[0].Size         = sizeof(CLIENT_ID);
                ps_attributes.Attributes[0].ValuePtr     = &client_id;
                ps_attributes.Attributes[0].ReturnLength = 0;

                void*      new_handle = nullptr;
                const auto ret =
                    LI_NT(NtCreateThreadEx)(&new_handle,
                                            THREAD_ALL_ACCESS,
                                            &obj_attributes,
                                            unwrap_handle(process_handle),
                                            reinterpret_cast<void*>(function_address),
                                            function_arg,
                                            flags,
                                            0, // ZeroBits
                                            0, // StackSize
                                            0, // MaxStackSize
                                            &ps_attributes);
                _handle.reset(new_handle);
                if(NT_SUCCESS(ret) && thread_id)
                    *thread_id = reinterpret_cast<std::uintptr_t>(client_id.UniqueThread);

                return ret;
            }

            NT_FN
            create(PUSER_THREAD_START_ROUTINE function_address,
                   void*                      function_arg = nullptr,
                   unsigned long   create_flags = THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER,
                   std::uintptr_t* thread_id    = nullptr) noexcept
            {
                return create_remote(NtCurrentProcess(),
                                     function_address,
                                     function_arg,
                                     create_flags,
                                     thread_id);
            }

            NT_FN open(ACCESS_MASK access, void* thread_id) noexcept
            {
                void*      handle = nullptr;
                CLIENT_ID  cid{ nullptr, thread_id };
                auto       attributes = make_attributes(nullptr, 0);
                const auto status =
                    LI_NT(NtOpenThread)(&handle, access, &attributes, &cid);

                _handle.reset(handle);
                return status;
            }

            NT_FN terminate() const noexcept
            {
                return LI_NT(NtTerminateThread)(_handle.get(), STATUS_SUCCESS);
            }

            NT_FN suspend() const noexcept
            {
                return LI_NT(NtSuspendThread)(_handle.get(), nullptr);
            }

            NT_FN resume() const noexcept
            {
                return LI_NT(NtResumeThread)(_handle.get(), nullptr);
            }

            NT_FN id(CLIENT_ID& id) const noexcept
            {
                THREAD_BASIC_INFORMATION info;
                const auto               status = LI_NT(NtQueryInformationThread)(
                    _handle.get(), ThreadBasicInformation, &info, sizeof(info), nullptr);
                if(NT_SUCCESS(status))
                    id = info.ClientId;
                return status;
            }

            NT_FN hide_from_debugger() const noexcept
            {
                return LI_NT(NtSetInformationThread)(
                    _handle.get(), ThreadHideFromDebugger, 0, 0);
            }
        };

    } // namespace detail

    using unique_thread = detail::basic_thread<unique_handle>;
    using thread_ref    = detail::basic_thread<handle_ref>;

} // namespace ntw::obj

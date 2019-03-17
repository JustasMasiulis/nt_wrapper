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

    class thread_access_options {
        ulong_t _value = 0;

    public:
        ulong_t value() const noexcept { return _value; }

        thread_access_options& all_access()
        {
            _value = THREAD_ALL_ACCESS;
            return *this;
        }

        thread_access_options& terminate()
        {
            _value |= THREAD_TERMINATE;
            return *this;
        }

        thread_access_options& suspend_resume()
        {
            _value |= THREAD_SUSPEND_RESUME;
            return *this;
        }

        thread_access_options& get_context()
        {
            _value |= THREAD_GET_CONTEXT;
            return *this;
        }

        thread_access_options& set_context()
        {
            _value |= THREAD_SET_CONTEXT;
            return *this;
        }

        thread_access_options& query_info()
        {
            _value |= THREAD_QUERY_INFORMATION;
            return *this;
        }

        thread_access_options& set_info()
        {
            _value |= THREAD_SET_INFORMATION;
            return *this;
        }

        thread_access_options& set_token()
        {
            _value |= THREAD_SET_THREAD_TOKEN;
            return *this;
        }

        thread_access_options& impersonate()
        {
            _value |= THREAD_IMPERSONATE;
            return *this;
        }

        thread_access_options& direct_impersonation()
        {
            _value |= THREAD_DIRECT_IMPERSONATION;
            return *this;
        }

        thread_access_options& set_limited_info()
        {
            _value |= THREAD_SET_LIMITED_INFORMATION;
            return *this;
        }

        thread_access_options& query_limited_info()
        {
            _value |= THREAD_QUERY_LIMITED_INFORMATION;
            return *this;
        }

        thread_access_options& resume()
        {
            _value |= THREAD_RESUME;
            return *this;
        }

        thread_access_options& synchronize()
        {
            _value |= SYNCHRONIZE;
            return *this;
        }

        thread_access_options& delete_object()
        {
            _value |= DELETE;
            return *this;
        }

        thread_access_options& read_security_desc()
        {
            _value |= READ_CONTROL;
            return *this;
        }

        thread_access_options& write_dac()
        {
            _value |= WRITE_DAC;
            return *this;
        }

        thread_access_options& write_security_desc()
        {
            _value |= WRITE_OWNER;
            return *this;
        }
    };

    struct thread_options_data {
        void*              process        = NtCurrentProcess();
        OBJECT_ATTRIBUTES* attributes     = nullptr;
        ulong_t            flags          = 0;
        SIZE_T             zero_bytes     = 0;
        SIZE_T             stack_size     = 0;
        SIZE_T             max_stack_size = 0;
    };

    // TODO inl file
    class thread_options {
        thread_options_data _data;

    public:
        const thread_options_data& data() const noexcept { return _data; }

        template<class Handle>
        thread_options& process(const Handle& handle)
        {
            _data.process = unwrap_handle(handle);
            return *this;
        }

        thread_options& attributes(OBJECT_ATTRIBUTES& attr)
        {
            _data.attributes = &attr;
            if(attr.SecurityDescriptor)
                _data.flags |= THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR;
            return *this;
        }

        thread_options& hide_from_debugger()
        {
            _data.flags |= THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER;
            return *this;
        }

        thread_options& create_suspended()
        {
            _data.flags |= THREAD_CREATE_FLAGS_CREATE_SUSPENDED;
            return *this;
        }

        thread_options& skip_attach()
        {
            _data.flags |= THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH;
            return *this;
        }

        thread_options& access_check_in_target()
        {
            _data.flags |= THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET;
            return *this;
        }

        thread_options& is_initial()
        {
            _data.flags |= THREAD_CREATE_FLAGS_INITIAL_THREAD;
            return *this;
        }

        thread_options& zero_bytes(SIZE_T num_bytes)
        {
            _data.zero_bytes = num_bytes;
            return *this;
        }

        thread_options& stack_size(SIZE_T size)
        {
            _data.stack_size = size;
            return *this;
        }

        thread_options& max_stack_size(SIZE_T size)
        {
            _data.max_stack_size = size;
            return *this;
        }
    };

    template<class Handle>
    class basic_thread {
        Handle _handle{ NtCurrentThread() };

    public:
        NTW_INLINE basic_thread() noexcept = default;

        template<class ObjectHandle>
        NTW_INLINE basic_thread(const ObjectHandle& handle)
            : _handle(unwrap_handle(handle))
        {}

        NTW_INLINE Handle& handle() noexcept { return _handle; }
        NTW_INLINE const Handle& handle() const noexcept { return _handle; }

        NT_FN create(PUSER_THREAD_START_ROUTINE function,
                     void*                      function_arg = nullptr,
                     const thread_options&      options      = {},
                     thread_access_options      access       = {},
                     std::uintptr_t*            thread_id    = nullptr) noexcept
        {
            CLIENT_ID          client_id = {};
            PS_ATTRIBUTE_LIST  ps_attributes;
            PS_ATTRIBUTE_LIST* attributes_ptr = nullptr;
            if(thread_id) {
                attributes_ptr = &ps_attributes;

                ps_attributes.TotalLength = sizeof(PS_ATTRIBUTE_LIST);
                ps_attributes.Attributes[0].Attribute =
                    ProcThreadAttributeValue(PsAttributeClientId, TRUE, FALSE, FALSE);
                ps_attributes.Attributes[0].Size         = sizeof(CLIENT_ID);
                ps_attributes.Attributes[0].ValuePtr     = &client_id;
                ps_attributes.Attributes[0].ReturnLength = 0;
            }

            const auto& data        = options.data();
            void*       temp_handle = nullptr;
            const auto  status =
                LI_NT(NtCreateThreadEx)(&temp_handle,
                                        access.value(),
                                        data.attributes,
                                        data.process,
                                        function_address,
                                        function_arg,
                                        data.flags,
                                        data.zero_bytes, // ZeroBits
                                        data.stack_size, // StackSize
                                        data.max_stack_size, // MaxStackSize
                                        attributes_ptr);

            if(NT_SUCCESS(status)) {
                if(thread_id)
                    *thread_id = reinterpret_cast<std::uintptr_t>(client_id.UniqueThread);

                _handle.reset(temp_handle);
            }

            return status;
        }

        NT_FN open(ACCESS_MASK access, void* thread_id) noexcept
        {
            CLIENT_ID cid{ nullptr, thread_id };
            auto      attributes = make_attributes(nullptr, 0);

            void*      temp_handle = nullptr;
            const auto status =
                LI_NT(NtOpenThread)(&temp_handle, access, &attributes, &cid);

            if(NT_SUCCESS(status))
                _handle.reset(temp_handle);

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

        NT_FN context(CONTEXT& ctx, unsigned long flags = 0) const noexcept
        {
            if(flags)
                ctx.ContextFlags = flags;

            return LI_NT(NtGetContextThread)(_handle.get(), &ctx);
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

        template<class ProcessHandle>
        NT_FN next(ACCESS_MASK          desired_access,
                   const ProcessHandle& process,
                   unsigned long        attributes = 0,
                   unsigned long        flags      = 0) noexcept
        {
            void*      new_handle;
            const auto status = LI_NT(NtGetNextThread)(unwrap_handle(process),
                                                       _handle.get(),
                                                       desired_access,
                                                       attributes,
                                                       flags,
                                                       &new_handle);

            if(NT_SUCCESS(status))
                _handle.reset(new_handle);

            return status;
        }
    };


    using unique_thread = basic_thread<unique_handle>;
    using thread_ref    = basic_thread<handle_ref>;

} // namespace ntw::obj

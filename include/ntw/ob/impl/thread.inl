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

#include "../thread.hpp"

namespace ntw::ob {

    NTW_INLINE constexpr thread_access& thread_access::terminate() noexcept
    {
        _access |= THREAD_TERMINATE;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::suspend_resume() noexcept
    {
        _access |= THREAD_SUSPEND_RESUME;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::get_context() noexcept
    {
        _access |= THREAD_GET_CONTEXT;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::set_context() noexcept
    {
        _access |= THREAD_SET_CONTEXT;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::query() noexcept
    {
        _access |= THREAD_QUERY_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::set_info() noexcept
    {
        _access |= THREAD_SET_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::set_token() noexcept
    {
        _access |= THREAD_SET_THREAD_TOKEN;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::impersonate() noexcept
    {
        _access |= THREAD_IMPERSONATE;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::direct_impersonate() noexcept
    {
        _access |= THREAD_DIRECT_IMPERSONATION;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::set_limited_info() noexcept
    {
        _access |= THREAD_SET_LIMITED_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::query_limited()
    {
        _access |= THREAD_QUERY_LIMITED_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::resume()
    {
        _access |= THREAD_RESUME;
        return *this;
    }

    NTW_INLINE constexpr thread_access& thread_access::all() noexcept
    {
        _access |= THREAD_ALL_ACCESS;
        return *this;
    }


    template<class T>
    thread_builder<T>& thread_builder<T>::argument(void* arg)
    {
        _argument = arg;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::zero_bits(std::size_t count)
    {
        _zero_bits = count;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::stack_size(std::size_t size)
    {
        _stack_size = size;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::max_stack_size(std::size_t size)
    {
        _max_stack_size = size;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::suspended()
    {
        _flags |= THREAD_CREATE_FLAGS_CREATE_SUSPENDED;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::skip_attach()
    {
        _flags |= THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::hide_from_dbg()
    {
        _flags |= THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::has_security_desc()
    {
        _flags |= THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::access_check_in_target()
    {
        _flags |= THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::bypass_process_freeze()
    {
        _flags |= 0x40;
        return *this;
    }

    template<class T>
    thread_builder<T>& thread_builder<T>::initial_thread()
    {
        _flags |= THREAD_CREATE_FLAGS_INITIAL_THREAD;
        return *this;
    }

    template<class T>
    template<class Process>
    NTW_INLINE result<T>
               thread_builder<T>::remote(const Process&             process,
                              PUSER_THREAD_START_ROUTINE routine,
                              thread_access              access,
                              PS_ATTRIBUTE_LIST*         attr_list) const noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtCreateThreadEx)(&handle,
                                                          access.get(),
                                                          nullptr,
                                                          ::ntw::detail::unwrap(process),
                                                          routine,
                                                          _argument,
                                                          _flags,
                                                          _zero_bits,
                                                          _stack_size,
                                                          _max_stack_size,
                                                          attr_list);
        return { status, T{ handle } };
    }

    template<class T>
    template<class Process>
    NTW_INLINE result<T>
               thread_builder<T>::remote(const Process&             process,
                              PUSER_THREAD_START_ROUTINE routine,
                              thread_access              access,
                              const ntw::ob::attributes& attr,
                              PS_ATTRIBUTE_LIST*         attr_list) const noexcept
    {
        void*      handle;
        const auto status =
            NTW_SYSCALL(NtCreateThreadEx)(&handle,
                                          access.get(),
                                          const_cast<OBJECT_ATTRIBUTES*>(&attr.get()),
                                          ::ntw::detail::unwrap(process),
                                          routine,
                                          _argument,
                                          _flags,
                                          _zero_bits,
                                          _stack_size,
                                          _max_stack_size,
                                          attr_list);
        return { status, T{ handle } };
    }

    template<class T>
    NTW_INLINE result<T>
               thread_builder<T>::local(PUSER_THREAD_START_ROUTINE routine,
                             thread_access              access,
                             PS_ATTRIBUTE_LIST*         attr_list) const noexcept
    {
        return remote(NtCurrentProcess(), routine, access, attr_list);
    }

    template<class T>
    NTW_INLINE result<T>
               thread_builder<T>::local(PUSER_THREAD_START_ROUTINE routine,
                             thread_access              access,
                             const ntw::ob::attributes& attr,
                             PS_ATTRIBUTE_LIST*         attr_list) const noexcept
    {
        return remote(NtCurrentProcess(), routine, access, attr, attr_list);
    }


    template<class H>
    NTW_INLINE basic_thread<H>::basic_thread() : Handle(NtCurrentThread())
    {}

    template<class H>
    template<class ThreadIdType>
    NTW_INLINE result<basic_thread<H>> basic_thread<H>::open(
        ThreadIdType tid, thread_access access, const attributes& attr) noexcept
    {
        CLIENT_ID         cid{ nullptr, tid };
        OBJECT_ATTRIBUTES attr = attr.get();

        void*      handle = nullptr;
        const auto status = NTW_SYSCALL(NtOpenThread)(&handle, access.get(), &attr, &cid);

        return { status, handle };
    }

    template<class H>
    NTW_INLINE thread_builder<basic_thread<H>> basic_thread<H>::create() noexcept
    {
        return {};
    }


    template<class H>
    NTW_INLINE status basic_thread<H>::terminate(status s) const noexcept
    {
        return NTW_SYSCALL(NtTerminateThread)(this->get(), s);
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::suspend() const noexcept
    {
        return NTW_SYSCALL(NtSuspendThread)(this->get(), nullptr);
    }

    template<class H>
    NTW_INLINE result<ntw::ulong_t> basic_thread<H>::suspend_with_prev() const noexcept
    {
        ntw::ulong_t prev_count;
        const auto   status = NTW_SYSCALL(NtSuspendThread)(this->get(), &prev_count);
        return { status, prev_count };
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::resume() const noexcept
    {
        return NTW_SYSCALL(NtResumeThread)(this->get(), nullptr);
    }

    template<class H>
    NTW_INLINE result<ntw::ulong_t> basic_thread<H>::resume_with_prev() const noexcept
    {
        ntw::ulong_t prev_count;
        const auto   status = NTW_SYSCALL(NtResumeThread)(this->get(), &prev_count);
        return { status, prev_count };
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::alert() const noexcept
    {
        return NTW_SYSCALL(NtTestAlert)(this->get());
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::alert_resume() const noexcept
    {
        return NTW_SYSCALL(NtAlertResumeThread)(this->get(), nullptr);
    }

    template<class H>
    NTW_INLINE result<ntw::ulong_t>
               basic_thread<H>::alert_resume_with_prev() const noexcept
    {
        ntw::ulong_t prev_count;
        const auto   status = NTW_SYSCALL(NtAlertResumeThread)(this->get(), &prev_count);
        return { status, prev_count };
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::queue_apc(PPS_APC_ROUTINE routine,
                                                 void*           arg1,
                                                 void*           arg2,
                                                 void*           arg3) const noexcept
    {
        return NTW_SYSCALL(NtQueueApcThread)(this->get(), routine, arg1, arg2, arg3);
    }

    template<class H>
    template<class ReserveHandle>
    NTW_INLINE status
    basic_thread<H>::queue_apc_with_reserve(const ReserveHandle& reserve,
                                            PPS_APC_ROUTINE      routine,
                                            void*                arg1,
                                            void*                arg2,
                                            void*                arg3) const noexcept
    {
        return NTW_SYSCALL(NtQueueApcThreadEx)(
            this->get(), ::ntw::detail::unwrap(reserve), routine, arg1, arg2, arg3);
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::queue_apc_force_signal(PPS_APC_ROUTINE routine,
                                                              void*           arg1,
                                                              void*           arg2,
                                                              void* arg3) const noexcept
    {
        return NTW_SYSCALL(NtQueueApcThreadEx)(
            this->get(), APC_FORCE_THREAD_SIGNAL, routine, arg1, arg2, arg3);
    }

    template<class H>
    NTW_INLINE result<basic_thread<H>>
               basic_thread<H>::first(thread_access access, attribute_options opt) noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtGetNextThread)(
            NtCurrentProcess(), nullptr, access.get(), opt.get(), 0, &handle);
        return { status, basic_thread{ handle } };
    }

    template<class H>
    template<class Process>
    NTW_INLINE result<basic_thread<H>> basic_thread<H>::first(
        const Process& process, thread_access access, attribute_options opt) noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtGetNextThread)(::ntw::details::unwrap(process),
                                                         nullptr,
                                                         access.get(),
                                                         opt.get(),
                                                         0,
                                                         &handle);
        return { status, basic_thread{ handle } };
    }

    template<class H>
    NTW_INLINE result<basic_thread<H>>
               basic_thread<H>::next(thread_access access, attribute_options opt) const noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtGetNextThread)(
            NtCurrentProcess(), this->get(), access.get(), opt.get(), 0, &handle);
        return { status, basic_thread{ handle } };
    }

    template<class H>
    template<class Process>
    NTW_INLINE result<basic_thread<H>>
               basic_thread<H>::next(const Process&    process,
                          thread_access     access,
                          attribute_options opt) const noexcept
    {
        void*      handle;
        const auto status = NTW_SYSCALL(NtGetNextThread)(::ntw::details::unwrap(process),
                                                         this->get(),
                                                         access.get(),
                                                         opt.get(),
                                                         0,
                                                         &handle);
        return { status, basic_thread{ handle } };
    }

    template<class H>
    NTW_INLINE result<CONTEXT> basic_thread<H>::context(ntw::ulong_t flags) const noexcept
    {
        ntw::result<CONTEXT> res;
        res->ContextFlags = flags;
        res.status()      = NTW_SYSCALL(NtGetContextThread)(this->get(), &*res);
        return res;
    }

    template<class H>
    NTW_INLINE status basic_thread<H>::context(const CONTEXT& ctx) const noexcept
    {
        return NTW_SYSCALL(NtSetContextThread)(this->get(), &const_cast<CONTEXT&>(ctx));
    }

} // namespace ntw::ob
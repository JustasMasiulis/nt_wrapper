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
#include "../access.hpp"

namespace ntw::ob {

    /// \brief Extends access_builder to contain all process specific access flags.
    struct thread_access : access_builder<thread_access> {
        /// \brief Enables THREAD_TERMINATE flag
        NTW_INLINE constexpr thread_access& terminate() noexcept;

        /// \brief Enables THREAD_SUSPEND_RESUME flag
        NTW_INLINE constexpr thread_access& suspend_resume() noexcept;

        /// \brief Enables THREAD_GET_CONTEXT flag
        NTW_INLINE constexpr thread_access& get_context() noexcept;

        /// \brief Enables THREAD_SET_CONTEXT flag
        NTW_INLINE constexpr thread_access& set_context() noexcept;

        /// \brief Enables THREAD_QUERY_INFORMATION flag
        NTW_INLINE constexpr thread_access& query() noexcept;

        /// \brief Enables THREAD_SET_INFORMATION flag
        NTW_INLINE constexpr thread_access& set_info() noexcept;

        /// \brief Enables THREAD_SET_THREAD_TOKEN flag
        NTW_INLINE constexpr thread_access& set_token() noexcept;

        /// \brief Enables THREAD_IMPERSONATE flag
        NTW_INLINE constexpr thread_access& impersonate() noexcept;

        /// \brief Enables THREAD_DIRECT_IMPERSONATION flag
        NTW_INLINE constexpr thread_access& direct_impersonate() noexcept;

        /// \brief Enables THREAD_SET_LIMITED_INFORMATION flag
        NTW_INLINE constexpr thread_access& set_limited_info() noexcept;

        /// \brief Enables THREAD_QUERY_LIMITED_INFORMATION flag
        NTW_INLINE constexpr thread_access& query_limited();

        /// \brief Enables THREAD_RESUME flag
        NTW_INLINE constexpr thread_access& resume();

        /// \brief Enables THREAD_ALL_ACCESS flag
        NTW_INLINE constexpr thread_access& all() noexcept;
    };

    template<class Thread>
    class thread_builder {
        void*        _argument       = nullptr;
        ntw::ulong_t _flags          = 0;
        SIZE_T       _zero_bits      = 0;
        SIZE_T       _stack_size     = 0;
        SIZE_T       _max_stack_size = 0;

    public:
        /// \brief Sets the thread start routine argument
        thread_builder& argument(void* arg);

        /// \brief Sets the amount of zero bits expected in thread stack address.
        thread_builder& zero_bits(std::size_t count);

        /// \brief Sets the initial size of thread stack.
        thread_builder& stack_size(std::size_t size);

        /// \brief Sets the maximum size of thread stack
        thread_builder& max_stack_size(std::size_t size);

        /// \brief Enables THREAD_CREATE_FLAGS_CREATE_SUSPENDED.
        thread_builder& suspended();

        /// \brief Enables THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH.
        thread_builder& skip_attach();

        /// \brief Enables THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER.
        thread_builder& hide_from_dbg();

        /// \brief Enables THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR.
        thread_builder& has_security_desc();

        /// \brief Enables THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET.
        thread_builder& access_check_in_target();

        /// \brief Enables BypassProcessFreeze flag.
        /// \note available since 19TH1
        thread_builder& bypass_process_freeze();

        /// \brief Enables THREAD_CREATE_FLAGS_INITIAL_THREAD flag.
        /// \note As far as I am aware this flag has no effect.
        thread_builder& initial_thread();

        /// \brief Creates a thread in the given process.
        /// \param routine The start address of the thread.
        /// \param access The requested access to the created thread object.
        /// \param attr_list The attributes list for the thread.
        template<class Process>
        NTW_INLINE result<Thread>
                   remote(const Process&             process,
                          PUSER_THREAD_START_ROUTINE routine,
                          thread_access              access = thread_access{}.maximum_allowed(),
                          PS_ATTRIBUTE_LIST*         attr_list = nullptr) const noexcept;

        /// \brief Creates a thread in the given process.
        /// \param routine The start address of the thread.
        /// \param access The requested access to the created thread object.
        /// \param attr The object attributes for the thread.
        /// \param attr_list The attributes list for the thread.
        template<class Process>
        NTW_INLINE result<Thread>
                   remote(const Process&             process,
                          PUSER_THREAD_START_ROUTINE routine,
                          thread_access              access,
                          const ntw::ob::attributes& attr,
                          PS_ATTRIBUTE_LIST*         attr_list = nullptr) const noexcept;

        /// \brief Creates a thread in the current process.
        /// \param routine The start address of the thread.
        /// \param access The requested access to the created thread object.
        /// \param attr_list The attributes list for the thread.
        NTW_INLINE result<Thread> local(
            PUSER_THREAD_START_ROUTINE routine,
            thread_access              access    = thread_access{}.maximum_allowed(),
            PS_ATTRIBUTE_LIST*         attr_list = nullptr) const noexcept;

        /// \brief Creates a thread in the current process.
        /// \param routine The start address of the thread.
        /// \param access The requested access to the created thread object.
        /// \param attr The object attributes for the thread.
        /// \param attr_list The attributes list for the thread.
        NTW_INLINE result<Thread> local(
            PUSER_THREAD_START_ROUTINE routine,
            thread_access              access,
            const ntw::ob::attributes& attr,
            PS_ATTRIBUTE_LIST*         attr_list = nullptr) const noexcept;
    };

    /// \brief Wrapper class around process object and its information
    template<class Handle>
    struct basic_thread : Handle {
        /// \brief The type of handle that is used internally
        using handle_type = Handle;
        using access_type = thread_access;

        /// \brief Inherits constructors from handle type.
        using handle_type::handle_type;
        using handle_type::operator=;

        /// \brief Constructs thread with the current thread handle.
        NTW_INLINE basic_thread();

        /// \brief Opens thread using given thread id, access and attributes
        /// \param tid Thread ID of any type convertible to void*
        /// \param access The access to request for when opening thread.
        /// \param attr Optional extra attributes.
        template<class ThreadIdType>
        NTW_INLINE static result<basic_thread> open(ThreadIdType      tid,
                                                    thread_access     access,
                                                    const attributes& attr = {}) noexcept;

        NTW_INLINE static thread_builder<basic_thread> create() noexcept;

        /// \brief Terminates thread using NtTerminateThread.
        NTW_INLINE status terminate(status s) const noexcept;

        /// \brief Suspends thread using NtSuspendThread.
        NTW_INLINE status suspend() const noexcept;

        /// \brief Suspends thread using NtSuspendThread.
        /// \returns Previous suspend count.
        NTW_INLINE result<ntw::ulong_t> suspend_with_prev() const noexcept;

        /// \brief Resumes thread using NtResumeThread.
        NTW_INLINE status resume() const noexcept;

        /// \brief Resumes thread using NtResumeThread.
        /// \returns Previous suspend count.
        NTW_INLINE result<ntw::ulong_t> resume_with_prev() const noexcept;

        /// \brief Alerts and thread using NtAlertThread.
        NTW_INLINE status alert() const noexcept;

        /// \brief Alerts and resumes thread using NtAlertResumeThread.
        NTW_INLINE status alert_resume() const noexcept;

        /// \brief Alerts and resumes thread using NtAlertResumeThread.
        /// \returns The previous suspend count.
        NTW_INLINE result<ntw::ulong_t> alert_resume_with_prev() const noexcept;

        /// \brief Queues an APC to the thread using NtQueueApcThread
        NTW_INLINE status queue_apc(PPS_APC_ROUTINE routine,
                                    void*           arg1 = nullptr,
                                    void*           arg2 = nullptr,
                                    void*           arg3 = nullptr) const noexcept;

        /// \brief Queues an APC to the thread with an APC reserve handle using
        /// NtQueueApcThreadEx
        template<class ReserveHandle>
        NTW_INLINE status queue_apc_with_reserve(const ReserveHandle& reserve,
                                                 PPS_APC_ROUTINE      routine,
                                                 void*                arg1 = nullptr,
                                                 void*                arg2 = nullptr,
                                                 void* arg3 = nullptr) const noexcept;

        /// \brief Queues an APC to the thread with APC_FORCE_THREAD_SIGNAL using
        /// NtQueueApcThreadEx
        NTW_INLINE status queue_apc_force_signal(PPS_APC_ROUTINE routine,
                                                 void*           arg1 = nullptr,
                                                 void*           arg2 = nullptr,
                                                 void* arg3 = nullptr) const noexcept;

        /// \brief Gets the first thread in current process.
        NTW_INLINE static result<basic_thread> first(thread_access     access,
                                                     attribute_options opt = {}) noexcept;

        /// \brief Gets the first thread in given process.
        template<class Process>
        NTW_INLINE static result<basic_thread> first(const Process&    process,
                                                     thread_access     access,
                                                     attribute_options opt = {}) noexcept;

        /// \brief Gets the next thread in current process.
        /// \returns The thread or STATUS_NO_MORE_ENTRIES if there are no more threads.
        NTW_INLINE result<basic_thread> next(thread_access     access,
                                             attribute_options opt = {}) const noexcept;

        /// \brief Gets the next thread in given process.
        /// \returns The thread or STATUS_NO_MORE_ENTRIES if there are no more threads.
        template<class Process>
        NTW_INLINE result<basic_thread> next(const Process&    process,
                                             thread_access     access,
                                             attribute_options opt = {}) const noexcept;

        /// \brief Gets the thread context.
        NTW_INLINE result<CONTEXT> context(ntw::ulong_t flags) const noexcept;

        /// \brief Sets the thread context.
        NTW_INLINE status context(const CONTEXT& ctx) const noexcept;
    };

    using thread     = basic_thread<object>;
    using thread_ref = basic_thread<object_ref>;

} // namespace ntw::ob

#include "impl/thread.inl"

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
#include "../job.hpp"
#include "../../detail/unwrap.hpp"

namespace ntw::ob {

    NTW_INLINE constexpr job_access& job_access::assign_process()
    {
        _access |= JOB_OBJECT_ASSIGN_PROCESS;
        return *this;
    }

    NTW_INLINE constexpr job_access& job_access::set_attributes()
    {
        _access |= JOB_OBJECT_SET_ATTRIBUTES;
        return *this;
    }

    NTW_INLINE constexpr job_access& job_access::query()
    {
        _access |= JOB_OBJECT_QUERY;
        return *this;
    }

    NTW_INLINE constexpr job_access& job_access::terminate()
    {
        _access |= JOB_OBJECT_TERMINATE;
        return *this;
    }

    NTW_INLINE constexpr job_access& job_access::set_security_attr()
    {
        _access |= JOB_OBJECT_SET_SECURITY_ATTRIBUTES;
        return *this;
    }

    NTW_INLINE constexpr job_access& job_access::impersonate()
    {
        _access |= JOB_OBJECT_IMPERSONATE;
        return *this;
    }

    NTW_INLINE constexpr job_access& job_access::all()
    {
        _access |= JOB_OBJECT_ALL_ACCESS;
        return *this;
    }

    template<class H>
    NTW_INLINE result<basic_job<H>> basic_job<H>::open(unicode_string    name,
                                                       job_access        access,
                                                       const attributes& attr) noexcept
    {
        OBJECT_ATTRIBUTES attributes = attr.get();
        attributes.ObjectName        = &name.get();
        void* handle;
        return { NTW_SYSCALL(NtOpenJobObject)(&handle, access.get(), &attributes),
                 basic_job{ handle } };
    }

    template<class H>
    NTW_INLINE result<basic_job<H>> basic_job<H>::create(job_access access) noexcept
    {
        void* handle;
        return { ntw::status{
                     NTW_SYSCALL(NtCreateJobObject)(&handle, access.get(), nullptr) },
                 basic_job{ handle } };
    }

    template<class H>
    NTW_INLINE result<basic_job<H>> basic_job<H>::create(unicode_string    name,
                                                         job_access        access,
                                                         const attributes& attr) noexcept
    {
        OBJECT_ATTRIBUTES attributes = attr.get();
        attributes.ObjectName        = &name.get();
        void* handle;
        return { NTW_SYSCALL(NtCreateJobObject)(&handle, access.get(), &attributes),
                 basic_job{ handle } };
    }

    template<class H>
    template<class ProcessHandle>
    NTW_INLINE status
    basic_job<H>::assign_process(const ProcessHandle& handle) const noexcept
    {
        return NTW_SYSCALL(NtAssignProcessToJobObject)(this->get(),
                                                       ntw::detail::unwrap(handle));
    }

    template<class H>
    NTW_INLINE status basic_job<H>::assign_curr_process() const noexcept
    {
        return NTW_SYSCALL(NtAssignProcessToJobObject)(this->get(), NtCurrentProcess());
    }

    template<class H>
    NTW_INLINE ntw::status basic_job<H>::terminate(ntw::status status) const noexcept
    {
        return NTW_SYSCALL(NtTerminateJobObject)(this->get(), status);
    }

    template<class H>
    template<class Process>
    NTW_INLINE result<bool>
               basic_job<H>::is_assigned(const Process& process) const noexcept
    {
        auto status =
            NTW_SYSCALL(NtIsProcessInJob)(::ntw::detail::unwrap(process), this->get());
        return { status, status != STATUS_PROCESS_NOT_IN_JOB };
    }

    template<class H>
    template<class T>
    NTW_INLINE ntw::status basic_job<H>::set(const T& info) const noexcept
    {
        static_assert(std::is_base_of_v<::ntw::detail::set_info, T>,
                      "given info class is not settable");

        return NTW_SYSCALL(NtSetInformationJobObject)(
            this->get(),
            info.info_class,
            ::std::addressof(const_cast<T&>(info).get()),
            sizeof(info));
    }

    template<class H>
    template<class T>
    NTW_INLINE ntw::result<T> basic_job<H>::query() const noexcept
    {
        static_assert(std::is_base_of_v<::ntw::detail::query_info, T>,
                      "given info class is not queryable");

        ntw::result<T> res;
        res.status() = NTW_SYSCALL(NtQueryInformationJobObject)(
            this->get(), T::info_class, ::std::addressof(res->get()), sizeof(T), nullptr);
        return res;
    }

} // namespace ntw::ob
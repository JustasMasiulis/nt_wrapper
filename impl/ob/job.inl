#pragma once
#include "../../include/ntw/ob/job.hpp"
#include "../../include/ntw/detail/unwrap.hpp"

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
        return NTW_SYSCALL(NtAssignProcessToJobObject)(
            this->get(), ntw::detail::unwrap_handle(handle));
    }

    template<class H>
    NTW_INLINE ntw::status basic_job<H>::terminate(ntw::status status) const noexcept
    {
        return NTW_SYSCALL(NtTerminateJobObject)(this->get(), status);
    }

} // namespace ntw::ob

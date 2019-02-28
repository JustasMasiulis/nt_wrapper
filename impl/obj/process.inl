#pragma once
#include "../../include/obj/process.hpp"

namespace ntw::obj {

    NTW_INLINE constexpr process_access& process_access::terminate()
    {
        _access |= PROCESS_TERMINATE;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::create_thread()
    {
        _access |= PROCESS_CREATE_THREAD;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::set_session_id()
    {
        _access |= PROCESS_SET_SESSIONID;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::vm_operation()
    {
        _access |= PROCESS_VM_OPERATION;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::vm_read()
    {
        _access |= PROCESS_VM_READ;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::vm_write()
    {
        _access |= PROCESS_VM_WRITE;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::dup_handle()
    {
        _access |= PROCESS_DUP_HANDLE;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::create_process()
    {
        _access |= PROCESS_CREATE_PROCESS;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::set_qouta()
    {
        _access |= PROCESS_SET_QUOTA;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::set_info()
    {
        _access |= PROCESS_SET_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::query_info()
    {
        _access |= PROCESS_QUERY_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::suspend_resume()
    {
        _access |= PROCESS_SUSPEND_RESUME;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::query_limited_info()
    {
        _access |= PROCESS_QUERY_LIMITED_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::set_limited_info()
    {
        _access |= PROCESS_SET_LIMITED_INFORMATION;
        return *this;
    }

    NTW_INLINE constexpr process_access& process_access::all()
    {
        _access = PROCESS_ALL_ACCESS;
        return *this;
    }

    template<class H>
    NTW_INLINE basic_process<H>::basic_process() : _handle(NtCurrentProcess())
    {}

    template<class H>
    template<class H2>
    NTW_INLINE basic_process<H>::basic_process(const H2& handle)
        : _handle(detail::unwrap_handle(handle))
    {}

    template<class H>
    NTW_INLINE H& basic_process<H>::handle()
    {
        return _handle;
    }

    template<class H>
    NTW_INLINE const H& basic_process<H>::handle() const
    {
        return _handle;
    }

    template<class H>
    template<class ProcessIdType>
    NTW_INLINE status basic_process<H>::open(ProcessIdType     pid,
                                             process_access    access,
                                             const attributes& attr)
    {
        CLIENT_ID cid{ reinterpret_cast<void*>(pid), nullptr };
        void*     result_handle;
        status    s = NTW_SYSCALL(NtOpenProcess)(&result_handle,
                                              access.get(),
                                              const_cast<OBJECT_ATTRIBUTES*>(&attr.get()),
                                              &cid);

        if(s.success())
            _handle.reset(result_handle);

        return s;
    }

} // namespace ntw::obj

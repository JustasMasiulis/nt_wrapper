#pragma once
#include "../../include/ntw/ob/process.hpp"
#include "../../include/ntw/detail/unwrap.hpp"

namespace ntw::ob {

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
    NTW_INLINE basic_process<H>::basic_process() : handle_type(NtCurrentProcess())
    {}

    template<class H>
    template<class ProcessIdType>
    NTW_INLINE result<basic_process<H>> basic_process<H>::open(
        ProcessIdType pid, process_access access, const attributes& attr) noexcept
    {
        CLIENT_ID cid{ reinterpret_cast<void*>(pid), nullptr };
        void*     result_handle = nullptr;
        status    s             = NTW_SYSCALL(NtOpenProcess)(&result_handle,
                                              access.get(),
                                              const_cast<OBJECT_ATTRIBUTES*>(&attr.get()),
                                              &cid);
        return { s, basic_process{ result_handle } };
    }

    template<class H>
    template<class Address, class Range>
    NTW_INLINE status basic_process<H>::read_mem(Address addr,
                                                 Range&& buffer) const noexcept
    {
        return NTW_SYSCALL(NtReadVirtualMemory)(
            this->get(),
            const_cast<void*>(reinterpret_cast<const void*>(addr)),
            static_cast<void*>(::ntw::detail::unfancy(::ntw::detail::adl_begin(buffer))),
            ::ntw::detail::range_byte_size(buffer),
            nullptr);
    }

    template<class H>
    template<class Address>
    NTW_INLINE status basic_process<H>::read_mem(Address     addr,
                                                 void*       buffer,
                                                 std::size_t size) const noexcept
    {
        return NTW_SYSCALL(NtReadVirtualMemory)(
            this->get(),
            const_cast<void*>(reinterpret_cast<const void*>(addr)),
            buffer,
            size,
            nullptr);
    }

    template<class H>
    template<class Address, class Range>
    NTW_INLINE status basic_process<H>::write_mem(Address addr,
                                                  Range&& buffer) const noexcept
    {
        return NTW_SYSCALL(NtWriteVirtualMemory)(
            this->get(),
            const_cast<void*>(reinterpret_cast<const void*>(addr)),
            const_cast<void*>(static_cast<const void*>(
                ::ntw::detail::unfancy(::ntw::detail::adl_begin(buffer)))),
            ::ntw::detail::range_byte_size(buffer),
            nullptr);
    }

    template<class H>
    template<class Address>
    NTW_INLINE status basic_process<H>::write_mem(Address     addr,
                                                  const void* buffer,
                                                  std::size_t size) const noexcept
    {
        return NTW_SYSCALL(NtWriteVirtualMemory)(
            this->get(),
            const_cast<void*>(reinterpret_cast<const void*>(addr)),
            const_cast<void*>(buffer),
            size,
            nullptr);
    }

    template<class H>
    template<class InfoType, class Address>
    NTW_INLINE result<InfoType> basic_process<H>::query_mem(Address addr) const noexcept
    {
        result<InfoType> res;
        res.status() = NTW_SYSCALL(NtQueryVirtualMemory)(
            this->get(),
            const_cast<void*>(reinterpret_cast<const void*>(addr)),
            InfoType::info_class,
            reinterpret_cast<typename InfoType::native_type*>(&*res),
            sizeof(typename InfoType::native_type),
            nullptr);
        return res;
    }

    template<class H>
    template<class Object, class Handle>
    NTW_INLINE result<Object>
               basic_process<H>::duplicate_object(const Handle& handle) const noexcept
    {
        void*  result;
        status s = NTW_SYSCALL(NtDuplicateObject)(this->get(),
                                                  ::ntw::detail::unwrap_handle(handle),
                                                  NtCurrentProcess(),
                                                  &result,
                                                  0,
                                                  0,
                                                  DUPLICATE_SAME_ACCESS |
                                                      DUPLICATE_SAME_ATTRIBUTES);

        return { s, Object{ result } };
    }

    template<class H>
    template<class Object, class Handle>
    NTW_INLINE result<Object>
               basic_process<H>::duplicate_object(const Handle&                handle,
                                       typename Object::access_type access,
                                       attribute_options            attr) const noexcept
    {
        void*  result;
        status s = NTW_SYSCALL(NtDuplicateObject)(this->get(),
                                                  ::ntw::detail::unwrap_handle(handle),
                                                  NtCurrentProcess(),
                                                  &result,
                                                  access.get(),
                                                  attr.get(),
                                                  0);

        return { s, Object{ result } };
    }

    template<class H>
    template<class Object, class Handle>
    NTW_INLINE result<Object> basic_process<H>::duplicate_object(
        const Handle& handle, typename Object::access_type access) const noexcept
    {
        void*  result;
        status s = NTW_SYSCALL(NtDuplicateObject)(this->get(),
                                                  ::ntw::detail::unwrap_handle(handle),
                                                  NtCurrentProcess(),
                                                  &result,
                                                  access.get(),
                                                  0,
                                                  DUPLICATE_SAME_ATTRIBUTES);

        return { s, Object{ result } };
    }

    template<class H>
    template<class Object, class Handle>
    NTW_INLINE result<Object> basic_process<H>::duplicate_object(
        const Handle& handle, attribute_options attr) const noexcept
    {
        void*  result;
        status s = NTW_SYSCALL(NtDuplicateObject)(this->get(),
                                                  ::ntw::detail::unwrap_handle(handle),
                                                  NtCurrentProcess(),
                                                  &result,
                                                  0,
                                                  attr.get(),
                                                  DUPLICATE_SAME_ACCESS);

        return { s, Object{ result } };
    }

    template<class H>
    template<class Handle>
    NTW_INLINE status basic_process<H>::close_object(const Handle& handle) const noexcept
    {
        return NTW_SYSCALL(NtDuplicateObject)(this->get(),
                                              ::ntw::detail::unwrap_handle(handle),
                                              nullptr,
                                              nullptr,
                                              0,
                                              nullptr,
                                              DUPLICATE_CLOSE_SOURCE);
    }

    template<class H>
    NTW_INLINE status basic_process<H>::suspend() const noexcept
    {
        return NTW_SYSCALL(NtSuspendProcess)(this->get());
    }

    template<class H>
    NTW_INLINE status basic_process<H>::resume() const noexcept
    {
        return NTW_SYSCALL(NtResumeProcess)(this->get());
    }

} // namespace ntw::ob

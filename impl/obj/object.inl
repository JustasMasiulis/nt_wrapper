#pragma once
#include "../include/obj/object.hpp"

namespace ntw::obj {

    template<class D>
    NTW_INLINE basic_object<D>::~basic_object()
    {
        reset();
    }

    template<class D>
    NTW_INLINE constexpr basic_object<D>::basic_object(void* handle) : _value(handle)
    {}

    template<class D>
    NTW_INLINE basic_object<D>::basic_object(basic_object&& other)
        : _value(other.release())
    {}

    template<class D>
    NTW_INLINE basic_object<D>& basic_object<D>::operator=(basic_object&& other)
    {
        reset(other.release());
        return *this;
    }

    template<class D>
    NTW_INLINE constexpr void* basic_object<D>::get() const
    {
        return _value;
    }

    template<class D>
    NTW_INLINE constexpr void** basic_object<D>::addressof()
    {
        return &_value;
    }

    template<class D>
    NTW_INLINE void basic_object<D>::reset(void* new_handle) noexcept
    {
        Deleter{}(_value);
        _value = new_handle;
    }

    template<class D>
    NTW_INLINE void basic_object<D>::reset() noexcept
    {
        reset(nullptr);
    }

    template<class D>
    NTW_INLINE constexpr void* basic_object<D>::release() noexcept
    {
        const auto temp = _value;
        _value          = nullptr;
        return temp;
    }

    template<class D>
    NTW_INLINE constexpr basic_object<D>::operator bool() const
    {
        return _value;
    }

    template<class D>
    template<class DOther>
    NTW_INLINE status basic_object<D>::_duplicate(void*                 process,
                                                  basic_object<DOther>& out,
                                                  unsigned long         access,
                                                  unsigned long         attributes,
                                                  unsigned long         options) const
    {
        void*  result;
        status s = NTW_SYSCALL(NtDuplicateObject)(
            process, _value, NtCurrentProcess(), &result, access, attributes, options);

        if(s.success())
            out.reset(result);

        return s;
    }

    template<class D>
    template<class Process, class DOther, class Access>
    NTW_INLINE status basic_object<D>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 Access                access,
                                                 attribute_options     attr) const
    {
        return _duplicate(
            detail::unwrap_handle(process), out, detail::unwrap(access), attr.get(), 0);
    }

    template<class D>
    template<class Process, class DOther>
    NTW_INLINE status basic_object<D>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 same_access_t,
                                                 attribute_options attr) const
    {
        return _duplicate(
            detail::unwrap_handle(process), out, 0, attr.get(), DUPLICATE_SAME_ACCESS);
    }

    template<class D>
    template<class Process, class DOther, class Access>
    NTW_INLINE status basic_object<D>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 Access                access,
                                                 same_attributes_t) const
    {
        return _duplicate(detail::unwrap_handle(process),
                          out,
                          detail::unwrap(access),
                          0,
                          DUPLICATE_SAME_ATTRIBUTES);
    }

    template<class D>
    template<class Process, class DOther>
    NTW_INLINE status basic_object<D>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 same_access_t,
                                                 same_attributes_t) const
    {
        return _duplicate(detail::unwrap_handle(process),
                          out,
                          0,
                          0,
                          DUPLICATE_SAME_ACCESS | DUPLICATE_SAME_ATTRIBUTES);
    }

    template<class D>
    template<class DOther, class Access, class Attributes>
    NTW_INLINE status basic_object<D>::duplicate(basic_object<DOther>& out,
                                                 Access                access,
                                                 Attributes            attr) const
    {
        return _duplicate(NtCurrentProcess(), out, access, attr, 0);
    }

    template<class D>
    template<class Process>
    NTW_INLINE status basic_object<D>::close_remote(const Process& process) const
    {
        return NTW_SYSCALL(NtDuplicateObject)(detail::unwrap_handle(process),
                                              _value,
                                              0,
                                              nullptr,
                                              0,
                                              0,
                                              DUPLICATE_CLOSE_SOURCE);
    }

    template<class D>
    NTW_INLINE status basic_object<D>::make_temporary() const
    {
        return NTW_SYSCALL(NtMakeTemporaryObject)(_value);
    }

    template<class D>
    NTW_INLINE status basic_object<D>::make_permanent() const
    {
        return NTW_SYSCALL(NtMakePermanentObject)(_value);
    }

    template<class D>
    NTW_INLINE status basic_object<D>::wait() const
    {
        return NTW_SYSCALL(NtWaitForSingleObject)(_value, false, nullptr);
    }

    template<class D>
    NTW_INLINE status basic_object<D>::wait(alertable_t) const
    {
        return NTW_SYSCALL(NtWaitForSingleObject)(_value, true, nullptr);
    }

    template<class D>
    NTW_INLINE status basic_object<D>::wait_for(nanosecond_hundreds timeout) const
    {
        LARGE_INTEGER li;
        li.QuadPart = -timeout.count();
        return NTW_SYSCALL(NtWaitForSingleObject)(_value, false, &li);
    }

    template<class D>
    NTW_INLINE status basic_object<D>::wait_for(nanosecond_hundreds timeout,
                                                alertable_t) const
    {
        LARGE_INTEGER li;
        li.QuadPart = -timeout.count();
        return NTW_SYSCALL(NtWaitForSingleObject)(_value, true, &li);
    }

} // namespace ntw::obj

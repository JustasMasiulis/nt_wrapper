#pragma once
#include "../include/ntw/ob/object.hpp"

namespace ntw::ob {

    template<class S>
    template<class SO>
    NTW_INLINE basic_object<S>& basic_object<S>::operator=(const basic_object<SO>& other)
    {
        *static_cast<S*>(this) = static_cast<const SO&>(other);
        return *this;
    }

    template<class S>
    template<class SO>
    NTW_INLINE basic_object<S>& basic_object<S>::operator=(basic_object<SO>&& other)
    {
        *static_cast<S*>(this) = std::move(*static_cast<SO*>(other));
        return *this;
    }

    template<class S>
    inline constexpr NTW_INLINE const S& basic_object<S>::storage() const
    {
        return *this;
    }

    template<class S>
    inline constexpr NTW_INLINE S& basic_object<S>::storage()
    {
        return *this;
    }

    template<class S>
    NTW_INLINE constexpr void basic_object<S>::reset(void* new_handle) noexcept
    {
        *static_cast<S*>(this) = S{ new_handle };
    }

    template<class S>
    NTW_INLINE constexpr void basic_object<S>::reset() noexcept
    {
        *static_cast<S*>(this) = nullptr;
    }

    template<class S>
    NTW_INLINE constexpr basic_object<S>::operator bool() const
    {
        return !!get();
    }

    template<class S>
    template<class DOther>
    NTW_INLINE status basic_object<S>::_duplicate(void*                 process,
                                                  basic_object<DOther>& out,
                                                  unsigned long         access,
                                                  unsigned long         attributes,
                                                  unsigned long         options) const
    {
        void*  result;
        status s = NTW_SYSCALL(NtDuplicateObject)(
            process, get(), NtCurrentProcess(), &result, access, attributes, options);

        if(s.success())
            out.reset(result);

        return s;
    }

    template<class S>
    template<class Process, class DOther, class Access>
    NTW_INLINE status basic_object<S>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 Access                access,
                                                 attribute_options     attr) const
    {
        return _duplicate(::ntw::detail::unwrap_handle(process),
                          out,
                          ::ntw::detail::unwrap(access),
                          attr.get(),
                          0);
    }

    template<class S>
    template<class Process, class DOther>
    NTW_INLINE status basic_object<S>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 same_access_t,
                                                 attribute_options attr) const
    {
        return _duplicate(::ntw::detail::unwrap_handle(process),
                          out,
                          0,
                          attr.get(),
                          DUPLICATE_SAME_ACCESS);
    }

    template<class S>
    template<class Process, class DOther, class Access>
    NTW_INLINE status basic_object<S>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 Access                access,
                                                 same_attributes_t) const
    {
        return _duplicate(::ntw::detail::unwrap_handle(process),
                          out,
                          ::ntw::detail::unwrap(access),
                          0,
                          DUPLICATE_SAME_ATTRIBUTES);
    }

    template<class S>
    template<class Process, class DOther>
    NTW_INLINE status basic_object<S>::duplicate(const Process&        process,
                                                 basic_object<DOther>& out,
                                                 same_access_t,
                                                 same_attributes_t) const
    {
        return _duplicate(::ntw::detail::unwrap_handle(process),
                          out,
                          0,
                          0,
                          DUPLICATE_SAME_ACCESS | DUPLICATE_SAME_ATTRIBUTES);
    }

    template<class S>
    template<class DOther, class Access, class Attributes>
    NTW_INLINE status basic_object<S>::duplicate(basic_object<DOther>& out,
                                                 Access                access,
                                                 Attributes            attr) const
    {
        return _duplicate(NtCurrentProcess(), out, access, attr, 0);
    }

    template<class S>
    template<class Process>
    NTW_INLINE status basic_object<S>::close_remote(const Process& process) const
    {
        return NTW_SYSCALL(NtDuplicateObject)(::ntw::detail::unwrap_handle(process),
                                              get(),
                                              0,
                                              nullptr,
                                              0,
                                              0,
                                              DUPLICATE_CLOSE_SOURCE);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::make_temporary() const
    {
        return NTW_SYSCALL(NtMakeTemporaryObject)(get());
    }

    template<class S>
    NTW_INLINE status basic_object<S>::make_permanent() const
    {
        return NTW_SYSCALL(NtMakePermanentObject)(get());
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait() const
    {
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), false, nullptr);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait(alertable_t) const
    {
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), true, nullptr);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait_for(duration timeout) const
    {
        LARGE_INTEGER li;
        li.QuadPart = -timeout.count();
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), false, &li);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait_for(duration timeout, alertable_t) const
    {
        LARGE_INTEGER li;
        li.QuadPart = -timeout.count();
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), true, &li);
    }


    namespace detail {

        NTW_INLINE constexpr object_ref_storage::object_ref_storage(
            std::nullptr_t) noexcept
            : _value(nullptr)
        {}

        template<class Handle>
        NTW_INLINE constexpr object_ref_storage::object_ref_storage(
            const Handle& other) noexcept
            : _value(::ntw::detail::unwrap_handle(other))
        {}

        template<class Handle>
        NTW_INLINE constexpr object_ref_storage&
        object_ref_storage::operator=(const Handle& other) noexcept
        {
            _value = ::ntw::detail::unwrap_handle(other);
            return *this;
        }

        NTW_INLINE constexpr void* object_ref_storage::get() const { return _value; }

        NTW_INLINE unique_object_storage::~unique_object_storage() noexcept
        {
            // -1 to -3 are predefined handles ( to -6 since win8 )
            if(reinterpret_cast<std::intptr_t>(_value) > 0 ||
               reinterpret_cast<std::intptr_t>(_value) < -3)
                // ignore return value
                static_cast<void>(NTW_SYSCALL(NtClose)(_value));
        }

        NTW_INLINE constexpr unique_object_storage::unique_object_storage(
            std::nullptr_t) noexcept
            : _value(nullptr)
        {}

        NTW_INLINE constexpr unique_object_storage::unique_object_storage(
            void* h) noexcept
            : _value(h)
        {}

        NTW_INLINE constexpr unique_object_storage::unique_object_storage(
            const object_ref_storage& ref) noexcept
            : _value(ref.get())
        {}

        NTW_INLINE constexpr unique_object_storage::unique_object_storage(
            unique_object_storage&& other) noexcept
            : _value(other._value)
        {
            other._value = nullptr;
        }

        NTW_INLINE constexpr unique_object_storage& unique_object_storage::operator=(
            unique_object_storage&& other) noexcept
        {
            const auto tmp = other._value;
            other._value   = _value;
            _value         = tmp;
            return *this;
        }

        NTW_INLINE constexpr unique_object_storage& unique_object_storage::operator=(
            std::nullptr_t) noexcept
        {
            *this = unique_object_storage{};
            return *this;
        }

        NTW_INLINE constexpr void* unique_object_storage::get() const { return _value; }

        NTW_INLINE constexpr void* unique_object_storage::release()
        {
            auto tmp = _value;
            _value   = nullptr;
            return tmp;
        }

    } // namespace detail

} // namespace ntw::ob

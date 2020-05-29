#pragma once
#include "../include/ntw/ob/object.hpp"

namespace ntw::ob {

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

} // namespace ntw::ob

#pragma once
#include "../include/ntw/result.hpp"

namespace ntw {

    template<class T>
    NTW_INLINE constexpr status& result<T>::status() noexcept
    {
        return *this;
    }

    template<class T>
    NTW_INLINE constexpr const status& result<T>::status() const noexcept
    {
        return *this;
    }

    template<class T>
    NTW_INLINE constexpr T& result<T>::operator*() noexcept
    {
        return _value;
    }

    template<class T>
    NTW_INLINE constexpr const T& result<T>::operator*() const noexcept
    {
        return _value;
    }

    template<class T>
    NTW_INLINE constexpr const T* ntw::result<T>::operator->() const noexcept
    {
        return &_value;
    }

    template<class T>
    NTW_INLINE constexpr T* ntw::result<T>::operator->() noexcept
    {
        return &_value;
    }

    template<class T>
    template<class Fn>
    NTW_INLINE constexpr auto ntw::result<T>::then(Fn fn)
        -> ntw::result<decltype(fn(**this))>
    {
        if(*this)
            return { status(), fn(**this) };
        return { status() };
    }

    template<class T>
    NTW_INLINE constexpr status& ntw::result_ref<T>::status() noexcept
    {
        return *this;
    }

    template<class T>
    NTW_INLINE constexpr const status& ntw::result_ref<T>::status() const noexcept
    {
        return *this;
    }

    template<class T>
    NTW_INLINE constexpr std::add_lvalue_reference_t<T>
        ntw::result_ref<T>::operator*() const
    {
        return *_value;
    }

    template<class T>
    NTW_INLINE constexpr T* ntw::result_ref<T>::operator->() const noexcept
    {
        return _value;
    }

} // namespace ntw
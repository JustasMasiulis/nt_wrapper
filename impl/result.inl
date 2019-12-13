#pragma once
#include "../include/ntw/result.hpp"

namespace ntw {

    template<class T>
    inline constexpr status& result<T>::status() noexcept
    {
        return *this;
    }

    template<class T>
    inline constexpr const status& result<T>::status() const noexcept
    {
        return *this;
    }

    template<class T>
    constexpr std::add_lvalue_reference_t<T> result<T>::operator*() const
    {
        return _value;
    }

    template<class T>
    inline constexpr T* ntw::result<T>::operator->() const noexcept
    {
        return &_value;
    }

    template<class T>
    inline constexpr status& ntw::result_ref<T>::status() noexcept
    {
        return *this;
    }

    template<class T>
    inline constexpr const status& ntw::result_ref<T>::status() const noexcept
    {
        return *this;
    }

    template<class T>
    constexpr std::add_lvalue_reference_t<T> ntw::result_ref<T>::operator*() const
    {
        return *_value;
    }

    template<class T>
    inline constexpr T* ntw::result_ref<T>::operator->() const noexcept
    {
        return _value;
    }

} // namespace ntw
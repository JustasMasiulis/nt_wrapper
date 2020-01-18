#pragma once
#include "macros.hpp"
#include <type_traits>

namespace ntw::detail {


    template<class, class = void>
    struct has_get : std::false_type {};

    template<class T>
    struct has_get<T, std::void_t<decltype(std::declval<T>().get())>> : std::true_type {};

    template<class, class = void>
    struct has_handle : std::false_type {};

    template<class T>
    struct has_handle<T, std::void_t<decltype(std::declval<T>().handle())>>
        : std::true_type {};

    template<class, class = void>
    struct has_addressof : std::false_type {};

    template<class T>
    struct has_addressof<T, std::void_t<decltype(std::declval<T>().addressof())>>
        : std::true_type {};


    template<class T>
    NTW_INLINE constexpr auto unwrap(const T& x) noexcept
    {
        if constexpr(has_get<T>::value)
            return x.get();
        else
            return x;
    }

    template<class T>
    NTW_INLINE constexpr auto unwrap_handle(const T& handle) noexcept
    {
        if constexpr(std::is_pointer_v<T> || std::is_null_pointer_v<T>)
            return handle;
        else if constexpr(has_get<T>::value)
            return handle.get();
        else
            return handle.handle().get();
    }

    template<class T>
    NTW_INLINE constexpr auto unwrap_handle_addressof(T& handle) noexcept
    {
        if constexpr(std::is_pointer_v<T>)
            return &handle;
        else if constexpr(has_addressof<T>::value)
            return handle.addressof();
        else
            return handle.handle().addressof();
    }

} // namespace ntw::detail
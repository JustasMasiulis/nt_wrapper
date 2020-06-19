#pragma once
#include "macros.hpp"
#include <type_traits>

namespace ntw::detail {

    template<class, class = void>
    struct has_get : std::false_type {};

    template<class T>
    struct has_get<T, std::void_t<decltype(std::declval<T>().get())>> : std::true_type {};

    template<class T>
    NTW_INLINE constexpr auto unwrap(const T& x) noexcept
    {
        if constexpr(has_get<T>::value)
            return x.get();
        else
            return x;
    }

} // namespace ntw::detail
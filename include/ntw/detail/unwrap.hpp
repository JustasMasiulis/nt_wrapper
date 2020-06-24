/*
 * Copyright 2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "config.hpp"
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
/*
 * Copyright 2018 Justas Masiulis
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
#include "detail/common.hpp"
#include <cstdint>

namespace ntw {

    template<class T, class Size>
    class basic_byte_span {
        T*   _ptr;
        Size _size;

        using void_pointer = std::conditional_t<std::is_const<T>, const void*, void*>;

    public:
        using value_type = T;
        using size_type  = Size;
        using iterator   = value_type*;
        using pointer    = value_type*;

        NTW_INLINE constexpr basic_byte_span(void_pointer p, size_type size) noexcept
            : _ptr(static_cast<pointer>(p)), _size(size)
        {}

        NTW_INLINE constexpr basic_byte_span(void_pointer begin,
                                             void_pointer end) noexcept
            : _ptr(static_cast<pointer>(begin))
        {
            // TODO add debug asserts
            const auto size = static_cast<pointer>(end) - static_cast<pointer>(begin);
            _size           = static_cast<size_type>(size);
        }

        NTW_INLINE iterator begin() const noexcept { return _begin; }
        NTW_INLINE iterator end() const noexcept { return _ptr + _size; }
        NTW_INLINE size_type size() const noexcept { return _size; }
    };

    template<class Size>
    using byte_span = basic_byte_span<std::uint8_t, Size>;

    template<class Size>
    using cbyte_span = basic_byte_span<const std::uint8_t, Size>;

} // namespace ntw

#include "impl/util.inl"

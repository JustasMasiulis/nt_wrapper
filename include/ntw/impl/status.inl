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
#include "../status.hpp"
#include <type_traits>

namespace ntw {

    NTW_INLINE constexpr status::status(std::int32_t status) : _value(status) {}

    NTW_INLINE constexpr status& status::operator=(std::int32_t status)
    {
        _value = status;
        return *this;
    }

    NTW_INLINE constexpr bool status::operator==(std::int32_t status) const
    {
        return _value == status;
    }

    NTW_INLINE constexpr status::operator bool() const { return _value < 0; }

    NTW_INLINE constexpr status::operator std::int32_t() const { return _value; }

    NTW_INLINE constexpr bool status::success() const { return _value >= 0; }

    NTW_INLINE constexpr bool status::information() const
    {
        return (static_cast<std::uint32_t>(_value) >> 30) == 1;
    }

    NTW_INLINE constexpr bool status::warning() const
    {
        return (static_cast<std::uint32_t>(_value) >> 30) == 2;
    }

    NTW_INLINE constexpr bool status::error() const
    {
        return (static_cast<std::uint32_t>(_value) >> 30) == 3;
    }


    NTW_INLINE constexpr severity status::severity() const
    {
        return static_cast<ntw::severity>(static_cast<std::uint32_t>(_value) >> 30);
    }

    NTW_INLINE constexpr facility status::facility() const
    {
        return static_cast<ntw::facility>((static_cast<std::uint32_t>(_value) << 4) >>
                                          20);
    }

    NTW_INLINE constexpr std::int32_t status::code() const
    {
        return static_cast<std::uint32_t>(_value) & 0xFFFF;
    }

    NTW_INLINE constexpr std::int32_t status::get() const { return _value; }

    template<class Fn>
    NTW_INLINE constexpr status& status::and_then(Fn f)
    {
        if(_value >= 0) {
            if constexpr(std::is_same<decltype(f(*this)), void>::value)
                f(*this);
            else
                _value = f(*this);
        }
        return *this;
    }

    template<class Fn>
    NTW_INLINE constexpr status& status::or_else(Fn f)
    {
        if(_value < 0) {
            if constexpr(std::is_same<decltype(f(*this)), void>::value)
                f(*this);
            else
                _value = f(*this);
        }
        return *this;
    }

} // namespace ntw
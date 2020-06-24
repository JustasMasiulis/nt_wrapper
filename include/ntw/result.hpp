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
#include <type_traits>
#include "status.hpp"

namespace ntw {

    template<class T>
    class result final : public ntw::status {
        using base_type = ntw::status;
        T _value;

    public:
        NTW_INLINE constexpr result() = default;

        NTW_INLINE constexpr result(const result&)  = default;
        NTW_INLINE constexpr result(result&& other) = default;

        NTW_INLINE constexpr result& operator=(const result&) = default;
        NTW_INLINE constexpr result& operator=(result&& other) = default;

        NTW_INLINE constexpr result(status s) : base_type(s) {}
        NTW_INLINE constexpr result(status s, T&& v) : base_type(s), _value(std::move(v))
        {}
        NTW_INLINE constexpr result(status s, const T& v) : base_type(s), _value(v) {}

        NTW_INLINE constexpr status&            status() noexcept;
        NTW_INLINE constexpr const ntw::status& status() const noexcept;

        NTW_INLINE constexpr const T& operator*() const noexcept;
        NTW_INLINE constexpr T&       operator*() noexcept;

        NTW_INLINE constexpr const T* operator->() const noexcept;
        NTW_INLINE constexpr T*       operator->() noexcept;

        NTW_INLINE constexpr operator bool() const noexcept { return success(); }

        template<class Fn>
        NTW_INLINE constexpr auto then(Fn fn) -> ntw::result<decltype(fn(**this))>;
    };

    template<class T>
    class result_ref final : public status {
        using base_type = ntw::status;
        T* _value       = nullptr;

    public:
        NTW_INLINE constexpr result_ref() = default;

        template<class U>
        NTW_INLINE constexpr result_ref(status s, T* v) : base_type(s), _value(v)
        {}

        NTW_INLINE constexpr status&            status() noexcept;
        NTW_INLINE constexpr const ntw::status& status() const noexcept;

        NTW_INLINE constexpr std::add_lvalue_reference_t<T> operator*() const;
        NTW_INLINE constexpr T*                             operator->() const noexcept;
    };

} // namespace ntw

#include "impl/result.inl"
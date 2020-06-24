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
#include "../result.hpp"

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
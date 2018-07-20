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
#include "common.hpp"

namespace ntw { namespace detail {

    template<class Traits>
    class unique_object : public Traits {
        typename Traits::value_type* _handle = nullptr;

    public:
        using value_type = typename Traits::value_type;
        using pointer    = value_type*;

        NTW_INLINE unique_object() noexcept = default;
        NTW_INLINE ~unique_object() noexcept { Traits::close(_handle); }

        NTW_INLINE unique_object(pointer handle) noexcept : _handle(handle) {}

        unique_object(const unique_object&) = delete;
        NTW_INLINE unique_object(unique_object&& other) noexcept
            : _handle(other.release())
        {}

        unique_object& operator=(const unique_object&) = delete;
        NTW_INLINE unique_object& operator             =(unique_object&& other) noexcept
        {
            reset(other.release());
            return *this;
        }

        NTW_INLINE pointer get() const noexcept { return _handle; }
        NTW_INLINE pointer* addressof() noexcept { return &_handle; }

        NTW_INLINE void reset(pointer new_handle) noexcept
        {
            Traits::close(_handle);
            _handle = new_handle;
        }
        NTW_INLINE void reset() noexcept
        {
            Traits::close(_handle);
            _handle = nullptr;
        }

        NTW_INLINE pointer release() noexcept
        {
            const auto temp = _handle;
            _handle         = nullptr;
            return temp;
        }

        NTW_INLINE std::add_lvalue_reference_t<value_type> operator*() const noexcept
        {
            return *get();
        }

        NTW_INLINE pointer operator->() const noexcept { return get(); }

        NTW_INLINE explicit operator bool() const noexcept { return _handle; }
    };

    template<class Traits>
    class ref_object : public Traits {
        typename Traits::value_type* _handle = nullptr;

    public:
        using value_type = typename Traits::value_type;
        using pointer    = value_type*;

        NTW_INLINE ref_object() noexcept = default;

        NTW_INLINE ref_object(pointer handle) noexcept : _handle(handle) {}

        NTW_INLINE pointer get() const noexcept { return _handle; }
        NTW_INLINE pointer* addressof() noexcept { return &_handle; }

        NTW_INLINE void reset(pointer new_handle) noexcept { _handle = new_handle; }
        NTW_INLINE void reset() noexcept { _handle = nullptr; }

        NTW_INLINE pointer release() noexcept
        {
            const auto temp = _handle;
            _handle         = nullptr;
            return temp;
        }

        NTW_INLINE std::add_lvalue_reference_t<value_type> operator*() const noexcept
        {
            return *get();
        }

        NTW_INLINE pointer operator->() const noexcept { return get(); }

        NTW_INLINE explicit operator bool() const noexcept { return _handle; }
    };

}} // namespace ntw::detail

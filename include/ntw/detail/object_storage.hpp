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
#include "common.hpp"
#include "unwrap.hpp"

namespace ntw::ob::detail {

    class object_ref_storage {
        void* _value = nullptr;

    public:
        /// \brief Constructs empty storage
        NTW_INLINE constexpr object_ref_storage() noexcept = default;

        /// \brief Constructs empty storage
        NTW_INLINE constexpr object_ref_storage(std::nullptr_t) noexcept;

        /// \brief Copies storage from an object, handle or pointer
        template<class Handle>
        NTW_INLINE explicit constexpr object_ref_storage(const Handle& other) noexcept;

        /// \brief Copies storage from an object, handle or pointer
        template<class Handle>
        NTW_INLINE constexpr object_ref_storage& operator=(const Handle& other) noexcept;

        /// \brief Returns the stored value
        NTW_INLINE constexpr void* get() const noexcept;

        /// \brief Returns the stored value and nulls it
        NTW_INLINE constexpr void* release() noexcept;
    };

    class unique_object_storage {
        void* _value = nullptr;

    public:
        NTW_INLINE ~unique_object_storage() noexcept;

        /// \brief Constructs storage which does not own a handle
        NTW_INLINE constexpr unique_object_storage() noexcept = default;

        /// \brief Constructs storage which does not own a handle
        NTW_INLINE constexpr unique_object_storage(std::nullptr_t) noexcept;

        /// \brief Constructs storage which owns h
        NTW_INLINE constexpr explicit unique_object_storage(void* h) noexcept;

        /// \brief Constructs storage which owns ref
        NTW_INLINE constexpr explicit unique_object_storage(
            const object_ref_storage& ref) noexcept;

        /// \brief Constructs storage by transferring ownership from other to *this
        NTW_INLINE constexpr unique_object_storage(
            unique_object_storage&& other) noexcept;

        /// \brief Transfers ownership from other to *this
        NTW_INLINE constexpr unique_object_storage& operator=(
            unique_object_storage&& other) noexcept;

        /// \brief Releases ownership
        NTW_INLINE constexpr unique_object_storage& operator=(std::nullptr_t) noexcept;

        /// \brief Returns the stored value
        NTW_INLINE constexpr void* get() const noexcept;

        /// \brief Releases ownership of the stored value
        NTW_INLINE constexpr void* release() noexcept;
    };


    NTW_INLINE constexpr object_ref_storage::object_ref_storage(std::nullptr_t) noexcept
        : _value(nullptr)
    {}

    template<class Handle>
    NTW_INLINE constexpr object_ref_storage::object_ref_storage(
        const Handle& other) noexcept
        : _value(::ntw::detail::unwrap(other))
    {}

    template<class Handle>
    NTW_INLINE constexpr object_ref_storage&
    object_ref_storage::operator=(const Handle& other) noexcept
    {
        _value = ::ntw::detail::unwrap(other);
        return *this;
    }

    NTW_INLINE constexpr void* object_ref_storage::get() const noexcept { return _value; }

    NTW_INLINE unique_object_storage::~unique_object_storage() noexcept
    {
        // -1 to -3 are predefined handles ( to -6 since win8 )
        if(reinterpret_cast<std::intptr_t>(_value) > 0 ||
           reinterpret_cast<std::intptr_t>(_value) < -3)
            // ignore return value
            static_cast<void>(NTW_SYSCALL(NtClose)(_value));
    }

    NTW_INLINE constexpr unique_object_storage::unique_object_storage(
        std::nullptr_t) noexcept
        : _value(nullptr)
    {}

    NTW_INLINE constexpr unique_object_storage::unique_object_storage(void* h) noexcept
        : _value(h)
    {}

    NTW_INLINE constexpr unique_object_storage::unique_object_storage(
        const object_ref_storage& ref) noexcept
        : _value(ref.get())
    {}

    NTW_INLINE constexpr unique_object_storage::unique_object_storage(
        unique_object_storage&& other) noexcept
        : _value(other._value)
    {
        other._value = nullptr;
    }

    NTW_INLINE constexpr unique_object_storage& unique_object_storage::operator=(
        unique_object_storage&& other) noexcept
    {
        const auto tmp = other._value;
        other._value   = _value;
        _value         = tmp;
        return *this;
    }

    NTW_INLINE constexpr unique_object_storage& unique_object_storage::operator=(
        std::nullptr_t) noexcept
    {
        *this = unique_object_storage{};
        return *this;
    }

    NTW_INLINE constexpr void* unique_object_storage::get() const noexcept
    {
        return _value;
    }

    NTW_INLINE constexpr void* unique_object_storage::release() noexcept
    {
        auto tmp = _value;
        _value   = nullptr;
        return tmp;
    }

} // namespace ntw::ob::detail
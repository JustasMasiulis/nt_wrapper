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
#include "../detail/common.hpp"
#include <cstdint>

namespace ntw::io {

    template<class Callback = void>
    class async_query : Callback {
        IO_STATUS_BLOCK _iosb;

        NTW_INLINE void _invoke() { static_cast<Callback> (*this)(*this); }

    public:
        NTW_INLINE async_query(const Callback& callback) : Callback(callback) {}

        NTW_INLINE NTSTATUS status() const noexcept { return _iosb.Status; }
        NTW_INLINE std::uintptr_t transferred() const noexcept
        {
            return _iosb.Information;
        }
        NTW_INLINE explicit operator bool() const noexcept
        {
            return status() != STATUS_PENDING;
        }

        NTW_INLINE IO_STATUS_BLOCK& status_block() noexcept { return _iosb; }

        NTW_INLINE constexpr void* event() const noexcept { return nullptr; }

        /// \brief Returns type erased pointer that will be passed to invoke in APC
        /// routine.
        NTW_INLINE void* reference() noexcept { return this; }
        /// \brief Invokes the callback after restoring the type of pointer received from
        /// reference
        NTW_INLINE static void on_completion(void* data)
        {
            static_cast<async_query*>(data)->_invoke();
        }
    };

    template<>
    class async_query<void> {
        IO_STATUS_BLOCK _iosb;

    public:
        NTW_INLINE NTSTATUS status() const noexcept { return _iosb.Status; }
        NTW_INLINE std::uintptr_t transferred() const noexcept
        {
            return _iosb.Information;
        }
        NTW_INLINE explicit operator bool() const noexcept
        {
            return status() != STATUS_PENDING;
        }

        NTW_INLINE IO_STATUS_BLOCK& status_block() noexcept { return _iosb; }

        NTW_INLINE constexpr void* event() const noexcept { return nullptr; }

        /// \brief Returns type erased pointer that will be passed to invoke in APC
        /// routine.
        NTW_INLINE void*                 reference() noexcept { return this; }
        NTW_INLINE constexpr static void on_completion(void*) noexcept {}
    };

    namespace detail {

        template<class Query>
        NTW_INLINE constexpr auto completion_routine() noexcept
        {
            return [](void* context, IO_STATUS_BLOCK* iosb, unsigned long) {
                Query::on_completion(context);
            };
        }

    } // namespace detail

} // namespace ntw::io

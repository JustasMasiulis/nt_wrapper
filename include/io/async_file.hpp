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
#include "../native_peb.hpp"
#include "base_file.hpp"
#include <atomic>

namespace ntw::io {

    template<class Callback = void>
    class async_query : Callback {
        IO_STATUS_BLOCK _iosb;

        NTW_INLINE void _invoke() { static_cast<Callback> (*this)(*this); }

    public:
        NTW_INLINE async_query(const Callback& callback) : Callback(callback) {}

        NTW_INLINE NTSTATUS status() const noexcept { return _iosb.Status; }
        NTW_INLINE std::uintptr_t transferred() const noexcept { return _iosb.Information; }
        NTW_INLINE explicit       operator bool() const noexcept
        {
            return status() != STATUS_PENDING;
        }

        NTW_INLINE IO_STATUS_BLOCK& status_block() noexcept { return _iosb; }

        /// \brief Returns type erased pointer that will be passed to invoke in APC routine.
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
        NTW_INLINE std::uintptr_t transferred() const noexcept { return _iosb.Information; }
        NTW_INLINE explicit       operator bool() const noexcept
        {
            return status() != STATUS_PENDING;
        }

        NTW_INLINE IO_STATUS_BLOCK& status_block() noexcept { return _iosb; }

        /// \brief Returns type erased pointer that will be passed to invoke in APC routine.
        NTW_INLINE void*                 reference() noexcept { return this; }
        NTW_INLINE constexpr static void on_completion(void*) noexcept {}
    };

    template<class Handle>
    struct async_file_traits {
        using handle_type = Handle;

        constexpr static auto options =
            file_options{}.full_access().share_all();

        constexpr static auto pipe_options =
            ntw::io::pipe_options{}.share_all().full_access().sync().byte_stream();
    };

    template<class Handle>
    struct async_file_traits {
        using handle_type  = Handle;
        using options_type = file_options;

        constexpr static auto options = file_options{}.full_access().share_all();

        NT_FN static open(void*&              handle,
                          OBJECT_ATTRIBUTES&  attributes,
                          const options_type& options,
                          unsigned long       disposition);
    };

    template<class Handle, class Traits = async_file_traits<Handle>>
    class basic_async_file : public detail::base_file<Traits> {
        using base_type = detail::base_file<Traits>;

    public:
        NTW_INLINE basic_async_file()  = default;
        NTW_INLINE ~basic_async_file() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_async_file(const ObjectHandle& handle)
            : base_type(unwrap_handle(handle))
        {}

        template<class QueryData>
        NT_FN write(const void*   data,
                    unsigned long size,
                    std::int64_t  offset,
                    QueryData&    query) const noexcept;

        template<class QueryData>
        NT_FN
        read(void* buffer, unsigned long size, std::int64_t offset, QueryData& query) const
            noexcept;

        NT_FN device_io_control(unsigned long  control_code,
                                const void*    in_buffer,
                                unsigned long  in_buffer_size,
                                void*          out_buffer,
                                unsigned long  out_buffer_size,
                                unsigned long* bytes_returned = nullptr) const noexcept;

        template<class InBuffer, class OutBuffer>
        NT_FN device_io_control(unsigned long   control_code,
                                const InBuffer& in_buffer,
                                OutBuffer&      out_buffer,
                                unsigned long*  bytes_returned = nullptr) const noexcept;
    };

    using unique_async_file = basic_async_file<unique_handle>;
    using async_file_ref    = basic_async_file<handle_ref>;

} // namespace ntw::io

#include "../impl/async_file.inl"

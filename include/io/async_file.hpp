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
#include "traits/file.hpp"
#include "async_query.hpp"

namespace ntw::io {

    template<class Handle, class Traits = traits::async_file_traits<Handle>>
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

        template<class QueryData>
        NT_FN device_io_control(unsigned long control_code,
                                const void*   in_buffer,
                                unsigned long in_buffer_size,
                                void*         out_buffer,
                                unsigned long out_buffer_size,
                                QueryData&    query) const noexcept;

        template<class InBuffer, class OutBuffer, class QueryData>
        NT_FN device_io_control(unsigned long   control_code,
                                const InBuffer& in_buffer,
                                OutBuffer&      out_buffer,
                                QueryData&      query) const noexcept;
    };

    using unique_async_file = basic_async_file<unique_handle>;
    using async_file_ref    = basic_async_file<handle_ref>;

} // namespace ntw::io

#include "../impl/async_file.inl"

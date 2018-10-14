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
#include "../byte_span.hpp"

namespace ntw::io {

    template<class Handle, class Traits = traits::file_traits<Handle>>
    class basic_file : public detail::base_file<Traits> {
        using base_type = detail::base_file<Traits>;

        template<class Fn, class QueryData>
        NT_FN _control(Fn                  fn,
                       ulong_t             control_code,
                       cbyte_span<ulong_t> input,
                       byte_span<ulong_t>  output,
                       ulong_t*            returned) const noexcept;

    public:
        NTW_INLINE basic_file() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_file(const ObjectHandle& handle)
            : base_type(unwrap_handle(handle))
        {}

        NT_FN write(cbyte_span<unsigned long> buffer,
                    std::int64_t              offset  = 0,
                    unsigned long*            written = nullptr) const noexcept;

        NT_FN read(byte_span<unsigned long> buffer,
                   std::int64_t             offset = 0,
                   unsigned long*           read   = nullptr) const noexcept;

        NT_FN device_io_control(unsigned long             control_code,
                                cbyte_span<unsigned long> input,
                                byte_span<unsigned long>  output,
                                unsigned long* returned = nullptr) const noexcept;

        template<class InBuffer, class OutBuffer>
        NT_FN device_io_control(unsigned long   control_code,
                                const InBuffer& input,
                                OutBuffer&      output,
                                unsigned long*  returned = nullptr) const noexcept;

        NT_FN fs_control(unsigned long             control_code,
                         cbyte_span<unsigned long> input,
                         byte_span<unsigned long>  output,
                         unsigned long*            returned = nullptr) const noexcept;

        template<class Input, class Output>
        NT_FN fs_control(unsigned long  control_code,
                         const Input&   input,
                         Output&        output,
                         unsigned long* returned = nullptr) const noexcept;
    };

    using unique_file = basic_file<unique_handle>;
    using file_ref    = basic_file<handle_ref>;


} // namespace ntw::io

#include "../impl/file.inl"

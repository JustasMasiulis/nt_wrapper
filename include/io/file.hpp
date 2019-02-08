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

    /// \brief A blocking file API.
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
        using basic_file::handle;

        NTW_INLINE basic_file() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_file(const ObjectHandle& handle)
            : base_type(unwrap_handle(handle))
        {}

        /// \brief Writes data to file using NtWriteFile API.
        /// \param buffer The data that will be written to the file.
        /// \param offset The offset from the beggining of file to write data to.
        /// \param read The number of bytes read.
        NT_FN write(cbyte_span<ulong_t> buffer,
                    std::int64_t        offset  = 0,
                    ulong_t*            written = nullptr) const noexcept;

        /// \brief Reads data from file using NtReadFile API.
        /// \param buffer The buffer into which the data will be read.
        /// \param offset The offset from the beggining of file to read data from.
        /// \param read The number of bytes read.
        NT_FN read(byte_span<ulong_t> buffer,
                   std::int64_t       offset = 0,
                   ulong_t*           read   = nullptr) const noexcept;

        /// \brief Sends a control code to a device driver using NtDeviceIoControl API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \param returned The number of bytes returned.
        NT_FN device_io_control(ulong_t             control_code,
                                cbyte_span<ulong_t> input,
                                byte_span<ulong_t>  output,
                                ulong_t*            returned = nullptr) const noexcept;

        /// \brief Sends a control code to a device driver using NtDeviceIoControl API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \param returned The number of bytes returned.
        template<class InBuffer, class OutBuffer>
        NT_FN device_io_control(ulong_t         control_code,
                                const InBuffer& input,
                                OutBuffer&      output,
                                ulong_t*        returned = nullptr) const noexcept;

        /// \brief Sends a control code to a file system or file system filter driver
        /// using NtFsControlFile API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \param returned The number of bytes returned.
        NT_FN fs_control(ulong_t             control_code,
                         cbyte_span<ulong_t> input,
                         byte_span<ulong_t>  output,
                         ulong_t*            returned = nullptr) const noexcept;

        /// \brief Sends a control code to a file system or file system filter driver
        /// using NtFsControlFile API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \param returned The number of bytes returned.
        template<class Input, class Output>
        NT_FN fs_control(ulong_t      control_code,
                         const Input& input,
                         Output&      output,
                         ulong_t*     returned = nullptr) const noexcept;
    };

    using unique_file = basic_file<unique_handle>;
    using file_ref    = basic_file<handle_ref>;


} // namespace ntw::io

#include "../impl/file.inl"

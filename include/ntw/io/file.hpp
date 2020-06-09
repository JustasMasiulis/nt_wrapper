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
#include "../detail/common.hpp"

namespace ntw::io {

    /// \brief A blocking file API.
    template<class Handle, class Traits = traits::file_traits<Handle>>
    struct basic_file : public detail::base_file<basic_file<Handle, Traits>, Traits> {
        using base_type = detail::base_file<basic_file<Handle, Traits>, Traits>;
        using base_type::handle;
        using base_type::base_type;
        using base_type::operator=;

        NTW_INLINE basic_file() = default;

        /// \brief Writes data to file using NtWriteFile API.
        /// \param buffer The data that will be written to the file.
        /// \param offset The offset from the beggining of file to write data to.
        /// \return Returns the amount of bytes written.
        NTW_INLINE result<ntw::ulong_t> write(cbyte_span   buffer,
                                              std::int64_t offset = 0) const noexcept;

        /// \brief Reads data from file using NtReadFile API.
        /// \param buffer The buffer into which the data will be read.
        /// \param offset The offset from the beggining of file to read data from.
        /// \return Returns the amount of bytes read.
        NTW_INLINE result<ntw::ulong_t> read(byte_span    buffer,
                                             std::int64_t offset = 0) const noexcept;

        /// \brief Sends a control code to a device driver using NtDeviceIoControl API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \return Returns the number of bytes written to output.
        NTW_INLINE result<ntw::ulong_t> device_io_control(
            ulong_t control_code, cbyte_span input, byte_span output) const noexcept;

        /// \brief Sends a control code to a device driver using NtDeviceIoControl API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \return Returns the number of bytes written to output.
        template<class InBuffer, class OutBuffer>
        NTW_INLINE result<ntw::ulong_t>
                   device_io_control(ulong_t         control_code,
                                     const InBuffer& input,
                                     OutBuffer&      output) const noexcept;

        /// \brief Sends a control code to a file system or file system filter driver
        /// using NtFsControlFile API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \return Returns the number of bytes written to output.
        NTW_INLINE result<ntw::ulong_t> fs_control(ulong_t    control_code,
                                                   cbyte_span input,
                                                   byte_span  output) const noexcept;

        /// \brief Sends a control code to a file system or file system filter driver
        /// using NtFsControlFile API.
        /// \param control_code The control code that will be sent.
        /// \param input The input buffer.
        /// \param output The output buffer.
        /// \return Returns the number of bytes written to output.
        template<class Input, class Output>
        NTW_INLINE result<ntw::ulong_t> fs_control(ulong_t      control_code,
                                                   const Input& input,
                                                   Output&      output) const noexcept;
    };

    using file     = basic_file<ob::object>;
    using file_ref = basic_file<ob::object_ref>;

} // namespace ntw::io

#include "../../../impl/io/file.inl"

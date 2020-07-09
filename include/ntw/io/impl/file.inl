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
#include "../file.hpp"

namespace ntw::io {

    template<class Handle, class Traits>
    NTW_INLINE result<ntw::ulong_t> basic_file<Handle, Traits>::write(
        cbyte_span buffer, std::int64_t offset) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   li_offset;
        li_offset.QuadPart = offset;

        const auto status =
            NTW_SYSCALL(NtWriteFile)(this->get(),
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     &status_block,
                                     const_cast<std::uint8_t*>(buffer.data()),
                                     buffer.size(),
                                     &li_offset,
                                     nullptr);
        return { status, static_cast<ulong_t>(status_block.Information) };
    }

    template<class Handle, class Traits>
    NTW_INLINE result<ntw::ulong_t>
               basic_file<Handle, Traits>::read(byte_span buffer, std::int64_t offset) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   li_offset;
        li_offset.QuadPart = offset;

        const auto status = NTW_SYSCALL(NtReadFile)(this->get(),
                                                    nullptr,
                                                    nullptr,
                                                    nullptr,
                                                    &status_block,
                                                    buffer.data(),
                                                    buffer.size(),
                                                    &li_offset,
                                                    nullptr);
        return { status, static_cast<ulong_t>(status_block.Information) };
    }

    template<class Handle, class Traits>
    NTW_INLINE result<ntw::ulong_t> basic_file<Handle, Traits>::device_io_control(
        ulong_t control_code, cbyte_span input, byte_span output) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        const auto      status =
            NTW_SYSCALL(NtDeviceIoControlFile)(this->get(),
                                               nullptr,
                                               nullptr,
                                               nullptr,
                                               &status_block,
                                               control_code,
                                               const_cast<std::uint8_t*>(input.begin()),
                                               input.size(),
                                               output.begin(),
                                               output.size());

        return { status, static_cast<ulong_t>(status_block.Information) };
    }

    template<class Handle, class Traits>
    template<class Input, class Output>
    NTW_INLINE result<ntw::ulong_t> basic_file<Handle, Traits>::device_io_control(
        ulong_t control_code, const Input& input, Output& output) const noexcept
    {
        return device_io_control(control_code,
                                 { ::std::addressof(input), sizeof(Input) },
                                 { ::std::addressof(output), sizeof(Output) });
    }

    template<class Handle, class Traits>
    NTW_INLINE result<ntw::ulong_t> basic_file<Handle, Traits>::fs_control(
        ulong_t control_code, cbyte_span input, byte_span output) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        const auto      status =
            NTW_SYSCALL(NtFsControlFile)(this->get(),
                                         nullptr,
                                         nullptr,
                                         nullptr,
                                         &status_block,
                                         control_code,
                                         const_cast<std::uint8_t*>(input.begin()),
                                         input.size(),
                                         output.begin(),
                                         output.size());

        return { status, static_cast<ulong_t>(status_block.Information) };
    }

    template<class Handle, class Traits>
    template<class Input, class Output>
    NTW_INLINE result<ntw::ulong_t> basic_file<Handle, Traits>::fs_control(
        ulong_t control_code, const Input& input, Output& output) const noexcept
    {
        return fs_control(control_code,
                          { ::std::addressof(input), sizeof(Input) },
                          { ::std::addressof(output), sizeof(Output) });
    }

} // namespace ntw::io
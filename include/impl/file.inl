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
#include "../io/file.hpp"

namespace ntw::io {

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::write(cbyte_span<unsigned long> buffer,
                                            std::int64_t              offset,
                                            unsigned long* written) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   li_offset = make_large_int(offset);

        const auto status = LI_NT(NtWriteFile)(handle().get(),
                                               nullptr,
                                               nullptr,
                                               nullptr,
                                               &status_block,
                                               const_cast<std::uint8_t*>(buffer.begin()),
                                               buffer.size(),
                                               &li_offset,
                                               nullptr);
        if(written && NT_SUCCESS(status))
            *written = static_cast<unsigned long>(status_block.Information);
        return status;
    }

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::read(byte_span<unsigned long> buffer,
                                           std::int64_t             offset,
                                           unsigned long*           read) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   li_offset = make_large_int(offset);
        const auto      status    = LI_NT(NtReadFile)(handle().get(),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              &status_block,
                                              buffer.begin(),
                                              buffer.size(),
                                              &li_offset,
                                              nullptr);
        if(read && NT_SUCCESS(status))
            *read = static_cast<unsigned long>(status_block.Information);
        return status;
    }

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::device_io_control(unsigned long control_code,
                                                        cbyte_span<unsigned long> input,
                                                        byte_span<unsigned long>  output,
                                                        unsigned long* returned) const
        noexcept
    {
        IO_STATUS_BLOCK status_block{ 0 };
        const auto      status =
            LI_NT(NtDeviceIoControlFile)(handle().get(),
                                         nullptr,
                                         nullptr,
                                         nullptr,
                                         &status_block,
                                         control_code,
                                         const_cast<std::uint8_t*>(input.begin()),
                                         input.size(),
                                         output.begin(),
                                         output.size());

        // set the bytes returned if we are successfull
        if(returned && NT_SUCCESS(status))
            *returned = static_cast<unsigned long>(status_block.Information);

        return status;
    }

    template<class Handle, class Traits>
    template<class Input, class Output>
    NT_FN basic_file<Handle, Traits>::device_io_control(unsigned long  control_code,
                                                        const Input&   input,
                                                        Output&        output,
                                                        unsigned long* returned) const
        noexcept
    {
        return device_io_control(control_code,
                                 { ::std::addressof(in_buffer), sizeof(InBuffer) },
                                 { ::std::addressof(out_buffer), sizeof(OutBuffer) },
                                 returned);
    }

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::fs_control(unsigned long             control_code,
                                                 cbyte_span<unsigned long> input,
                                                 byte_span<unsigned long>  output,
                                                 unsigned long* returned) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        const auto      status =
            LI_NT(NtFsControlFile)(handle().get(),
                                   nullptr,
                                   nullptr,
                                   nullptr,
                                   &status_block,
                                   control_code,
                                   const_cast<std::uint8_t*>(input.begin()),
                                   input.size(),
                                   output.begin(),
                                   output.size());

        // set the bytes returned if we are successfull
        if(returned && NT_SUCCESS(status))
            *returned = static_cast<unsigned long>(status_block.Information);

        return status;
    }

    template<class Handle, class Traits>
    template<class Input, class Output>
    NT_FN basic_file<Handle, Traits>::fs_control(unsigned long  control_code,
                                                 const Input&   input,
                                                 Output&        output,
                                                 unsigned long* returned) const noexcept
    {
        return fs_control(control_code,
                          { ::std::addressof(in_buffer), sizeof(InBuffer) },
                          { ::std::addressof(out_buffer), sizeof(OutBuffer) },
                          returned);
    }

} // namespace ntw::io

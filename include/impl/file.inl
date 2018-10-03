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
    NT_FN file_traits<Handle>::(void*&              handle,
                                OBJECT_ATTRIBUTES&  attributes,
                                const options_type& options,
                                unsigned long       disposition)
    {
        constexpr unsigned long extra_options =
            FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE;

        IO_STATUS_BLOCK status_block;
        return LI_NT(NtCreateFile)(&temp_handle,
                                   options._access | SYNCHRONIZE,
                                   &attributes,
                                   &status_block,
                                   nullptr,
                                   options._attributes ? options._attributes
                                                       : FILE_ATTRIBUTE_NORMAL,
                                   options._share_access,
                                   disposition,
                                   options._options | extra_options,
                                   nullptr,
                                   0);
    }

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::write(const void*    data,
                                            unsigned long  size,
                                            std::int64_t   offset,
                                            unsigned long* written) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   li_offset = make_large_int(offset);

        const auto status = LI_NT(NtWriteFile)(handle().get(),
                                               nullptr,
                                               nullptr,
                                               nullptr,
                                               &status_block,
                                               const_cast<void*>(data),
                                               size,
                                               &li_offset,
                                               nullptr);
        if(written && NT_SUCCESS(status))
            *written = static_cast<unsigned long>(status_block.Information);
        return status;
    }

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::read(void*          buffer,
                                           unsigned long  size,
                                           std::int64_t   offset,
                                           unsigned long* read) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   li_offset = make_large_int(offset);
        const auto      status    = LI_NT(NtReadFile)(handle().get(),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              &status_block,
                                              buffer,
                                              size,
                                              &li_offset,
                                              nullptr);
        if(read && NT_SUCCESS(status))
            *read = static_cast<unsigned long>(status_block.Information);
        return status;
    }

    template<class Handle, class Traits>
    NT_FN basic_file<Handle, Traits>::device_io_control(unsigned long  control_code,
                                                        const void*    in_buffer,
                                                        unsigned long  in_buffer_size,
                                                        void*          out_buffer,
                                                        unsigned long  out_buffer_size,
                                                        unsigned long* bytes_returned) const
        noexcept
    {
        IO_STATUS_BLOCK status_block{ 0 };
        const auto      status = LI_NT(NtDeviceIoControlFile)(handle().get(),
                                                         nullptr,
                                                         nullptr,
                                                         nullptr,
                                                         &status_block,
                                                         control_code,
                                                         const_cast<void*>(in_buffer),
                                                         in_buffer_size,
                                                         out_buffer,
                                                         out_buffer_size);

        // set the bytes returned if we are successfull
        if(bytes_returned && NT_SUCCESS(status))
            *bytes_returned = static_cast<unsigned long>(status_block.Information);

        return status;
    }

    template<class Handle, class Traits>
    template<class InBuffer, class OutBuffer>
    NT_FN basic_file<Handle, Traits>::device_io_control(unsigned long   control_code,
                                                        const InBuffer& in_buffer,
                                                        OutBuffer&      out_buffer,
                                                        unsigned long*  bytes_returned) const
        noexcept
    {
        return device_io_control(control_code,
                                 ::std::addressof(in_buffer),
                                 sizeof(InBuffer),
                                 ::std::addressof(out_buffer),
                                 sizeof(OutBuffer),
                                 bytes_returned);
    }


    template<class Callback, class... Args>
    NT_FN enum_directory(UNICODE_STRING name, Callback callback, Args&&... args)
    {
        ntw::unique_handle handle;

        {
            auto attributes = ntw::make_attributes(&name, 0);
            ret_on_err(LI_NT(NtOpenDirectoryObject)(
                handle.addressof(), DIRECTORY_QUERY | DIRECTORY_TRAVERSE, &attributes));
        }

        std::aligned_storage_t<2048u, 8> buffer;
        constexpr unsigned long          buffer_size = sizeof(buffer);
        unsigned long                    ctx = 0, ret_len = 0;

        while(true) {
            const auto status = LI_NT(NtQueryDirectoryObject)(
                handle.get(), &buffer, buffer_size, FALSE, FALSE, &ctx, &ret_len);

            if(status == STATUS_NO_MORE_ENTRIES)
                return STATUS_SUCCESS;
            else if(!NT_SUCCESS(status))
                return status;

            auto ptr = reinterpret_cast<OBJECT_DIRECTORY_INFORMATION*>(&buffer);
            // can't break out of 2 loops...
        goto_next_iteration:
            if(ptr->Name.Buffer != nullptr && ptr->TypeName.Buffer != nullptr) {
                NTW_CALLBACK_BREAK_IF_FALSE(callback, *ptr++, args...);
                goto goto_next_iteration;
            }
        }

        return STATUS_SUCCESS;
    }

} // namespace ntw::io

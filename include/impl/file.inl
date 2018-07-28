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
#include "../object/file.hpp"
#include "../util.hpp"

namespace ntw::obj::detail {

    template<class Handle>
    NT_FN basic_file<Handle>::_open(UNICODE_STRING path,
                                    bool           create_if_not_existing,
                                    unsigned long  access_flags,
                                    unsigned long  create_flags) noexcept
    {
        constexpr unsigned long share_flags =
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

        const unsigned long file_open_flags =
            (create_if_not_existing ? FILE_OPEN_IF : FILE_OPEN);

        access_flags |= SYNCHRONIZE;
        create_flags |= FILE_SYNCHRONOUS_IO_NONALERT;

        auto            attributes = make_attributes(&path, OBJ_CASE_INSENSITIVE);
        IO_STATUS_BLOCK status_block;
        _handle.reset();
        return LI_NT(NtCreateFile)(_handle.addressof(),
                                   access_flags,
                                   &attributes,
                                   &status_block,
                                   nullptr,
                                   FILE_ATTRIBUTE_NORMAL,
                                   share_flags,
                                   file_open_flags,
                                   create_flags,
                                   nullptr,
                                   0);
    }

    template<class Handle>
    template<class StringRef>
    NT_FN basic_file<Handle>::open_dir(const StringRef& path,
                                       bool             create_if_not_existing,
                                       unsigned long    access_flags) noexcept
    {
        return _open(
            make_ustr(path), create_if_not_existing, access_flags, FILE_DIRECTORY_FILE);
    }

    template<class Handle>
    template<class StringRef>
    NT_FN basic_file<Handle>::open(const StringRef& path,
                                   bool             create_if_not_existing,
                                   unsigned long    access_flags) noexcept
    {
        return _open(make_ustr(path),
                     create_if_not_existing,
                     access_flags,
                     FILE_NON_DIRECTORY_FILE);
    }

    template<class Handle>
    NT_FN basic_file<Handle>::open_as_pipe(UNICODE_STRING name,
                                           unsigned long  in_max,
                                           unsigned long  out_max,
                                           unsigned long  instances,
                                           LARGE_INTEGER  timeout) noexcept
    {
        // there are a few other flags we don't give a shit about
        constexpr unsigned long open_mode = GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE;

        auto            attributes = ntw::make_attributes(&name, OBJ_CASE_INSENSITIVE);
        IO_STATUS_BLOCK iosb;

        _handle.reset();
        // if you want to use MESSAGE bullshit you can't just use the same macros
        auto status = LI_NT(NtCreateNamedPipeFile)(_handle.addressof(),
                                                   open_mode,
                                                   &attributes,
                                                   &iosb,
                                                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                   FILE_OPEN_IF,
                                                   FILE_SYNCHRONOUS_IO_NONALERT,
                                                   PIPE_TYPE_BYTE,
                                                   PIPE_READMODE_BYTE,
                                                   PIPE_WAIT,
                                                   instances,
                                                   in_max,
                                                   out_max,
                                                   &timeout);

        // looking at IDA if iosb.Information == 1 ERROR_ALREADY_EXISTS ?
        if(NT_SUCCESS(status) && iosb.Information == 1)
            return STATUS_OBJECT_NAME_EXISTS;

        return status;
    }

    template<class Handle>
    NT_FN basic_file<Handle>::size(std::uint64_t& size_out) const noexcept
    {
        IO_STATUS_BLOCK           status_block;
        FILE_STANDARD_INFORMATION info;
        const auto                status = LI_NT(NtQueryInformationFile)(_handle.get(),
                                                          &status_block,
                                                          &info,
                                                          unsigned{ sizeof(info) },
                                                          FileStandardInformation);
        if(NT_SUCCESS(status))
            size_out = static_cast<std::uint64_t>(info.EndOfFile.QuadPart);

        return status;
    }
    template<class Handle>
    NT_FN basic_file<Handle>::write(const void*    data,
                                    unsigned long  size,
                                    unsigned long* written) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   offset{ 0 };

        const auto status = LI_NT(NtWriteFile)(_handle.get(),
                                               nullptr,
                                               nullptr,
                                               nullptr,
                                               &status_block,
                                               const_cast<void*>(data),
                                               size,
                                               &offset,
                                               nullptr);
        if(written && NT_SUCCESS(status))
            *written = static_cast<unsigned long>(status_block.Information);
        return status;
    }

    template<class Handle>
    NT_FN basic_file<Handle>::read(void*          buffer,
                                   unsigned long  size,
                                   unsigned long* read) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        LARGE_INTEGER   offset{ 0 };
        const auto      status = LI_NT(NtReadFile)(_handle.get(),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              &status_block,
                                              buffer,
                                              size,
                                              &offset,
                                              nullptr);
        if(read && NT_SUCCESS(status))
            *read = static_cast<unsigned long>(status_block.Information);
        return status;
    }

    template<class Handle>
    template<class Callback, class... Args>
    NT_FN basic_file<Handle>::enum_contained_files(Callback callback,
                                                   Args&&... args) const noexcept
    {
        std::aligned_storage_t<2048u, 8> buffer;
        constexpr unsigned long          buffer_size = sizeof(buffer);
        IO_STATUS_BLOCK                  iosb        = { 0 };

        while(true) {
            const auto status = LI_NT(NtQueryDirectoryFile)(_handle.get(),
                                                            nullptr,
                                                            nullptr,
                                                            nullptr,
                                                            &iosb,
                                                            &buffer,
                                                            buffer_size,
                                                            FileDirectoryInformation,
                                                            FALSE,
                                                            nullptr,
                                                            FALSE);
            // check the status if there are any more files
            if(!NT_SUCCESS(status)) {
                if(status == STATUS_NO_MORE_FILES)
                    return STATUS_SUCCESS;

                return status;
            }
            // I have a feeling this could introduce an infinite loop
            else if(iosb.Information == 0)
                continue;

            auto file_info = reinterpret_cast<FILE_DIRECTORY_INFORMATION*>(&buffer);

            // goto is pretty gay but I cannot use this macro with a nested loop
        goto_next_file:
            NTW_CALLBACK_BREAK_IF_FALSE(callback, *file_info, args...);

            // go to next file if the offset isnt 0, else get more infos
            if(file_info->NextEntryOffset) {
                file_info = reinterpret_cast<FILE_DIRECTORY_INFORMATION*>(
                    reinterpret_cast<std::uintptr_t>(file_info) +
                    file_info->NextEntryOffset);

                goto goto_next_file;
            }
        }

        return STATUS_SUCCESS;
    }

    template<class Handle>
    NT_FN basic_file<Handle>::device_io_control(unsigned long  control_code,
                                                void*          in_buffer,
                                                unsigned long  in_buffer_size,
                                                void*          out_buffer,
                                                unsigned long  out_buffer_size,
                                                unsigned long* bytes_returned) const
        noexcept
    {
        IO_STATUS_BLOCK status_block{ 0 };
        const auto      status = LI_NT(NtDeviceIoControlFile)(_handle.get(),
                                                         nullptr,
                                                         nullptr,
                                                         nullptr,
                                                         &status_block,
                                                         control_code,
                                                         in_buffer,
                                                         in_buffer_size,
                                                         out_buffer,
                                                         out_buffer_size);

        // set the bytes returned if we are successfull
        if(bytes_returned && NT_SUCCESS(status))
            *bytes_returned = static_cast<unsigned long>(status_block.Information);

        return status;
    }

    template<class Handle>
    template<class InBuffer, class OutBuffer>
    NT_FN basic_file<Handle>::device_io_control(unsigned long    control_code,
                                                const InBuffer&  in_buffer,
                                                const OutBuffer& out_buffer,
                                                unsigned long*   bytes_returned) const
        noexcept
    {
        return device_io_control(control_code,
                                 in_buffer,
                                 sizeof(InBuffer),
                                 out_buffer,
                                 sizeof(OutBuffer),
                                 bytes_returned);
    }

    template<class Handle>
    template<class StringRef /* wstring_view or UNICODE_STRING */>
    NT_FN static basic_file<Handle>::destroy(const StringRef& path,
                                             bool             case_sensitive) noexcept
    {
        auto upath = make_ustr(path);
        auto attributes =
            make_attributes(&upath, case_sensitive ? 0 : OBJ_CASE_INSENSITIVE);
        return LI_NT(NtDeleteFile)(&attributes);
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

} // namespace ntw::obj::detail
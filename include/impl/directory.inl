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
#include "../io/directory.hpp"

namespace ntw::io {

    template<class Handle, class Traits>
    template<class Callback, class... Args>
    NT_FN basic_directory<Handle, Traits>::enum_contents(void*    buffer_begin,
                                                         void*    buffer_end,
                                                         Callback cb,
                                                         Args&&... args) const noexcept
    {
        const auto buffer_size =
            static_cast<unsigned long>(static_cast<std::uint8_t*>(buffer_end) -
                                       static_cast<std::uint8_t*>(buffer_begin));

        IO_STATUS_BLOCK iosb = { 0 };

        bool restart_scan = true;

        while(true) {
            const auto status = LI_NT(NtQueryDirectoryFile)(_handle.get(),
                                                            nullptr,
                                                            nullptr,
                                                            nullptr,
                                                            &iosb,
                                                            buffer_begin,
                                                            buffer_size,
                                                            FileDirectoryInformation,
                                                            FALSE,
                                                            nullptr,
                                                            restart_scan);

            // after the first call we can se restart scan to false
            restart_scan = false;

            // check the status if there are any more files
            if(!NT_SUCCESS(status)) {
                if(status == STATUS_NO_MORE_FILES)
                    return STATUS_SUCCESS;

                return status;
            }
            // I have a feeling this could introduce an infinite loop
            else if(iosb.Information == 0)
                continue;

            auto file_info =
                std::launder(static_cast<FILE_DIRECTORY_INFORMATION*>(buffer_begin));

            // goto is bad but I cannot use this macro with a nested loop
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

    template<class Handle, class Traits>
    template<std::size_t StaticBufferSize, class Callback, class... Args>
    NT_FN basic_directory<Handle, Traits>::enum_contents(Callback&& cb,
                                                         Args&&... args) const noexcept
    {
        std::aligned_storage_t<StaticBufferSize, 8> buffer;
        return enum_contents(&buffer,
                             &buffer + 1,
                             std::forward<Callback>(cb),
                             std::forward<Args>(args)...);
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

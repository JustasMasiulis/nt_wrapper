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
#include "../detail/generic_query.hpp"

namespace ntw::system {

    template<class Buffer, NTW_QUERY_BUFFER_REQUIREMENT>
    NT_FN info(SYSTEM_INFORMATION_CLASS info_class,
               Buffer&                  buffer,
               unsigned long            size     = sizeof(Buffer),
               unsigned long*           returned = nullptr)
    {
        return LI_NT(NtQuerySystemInformation)(info_class, buffer, size, returned);
    }

    template<class Callback, class... Args, NTW_QUERY_CALLBACK_REQUIREMENT>
    NT_FN info(SYSTEM_INFORMATION_CLASS info_class, Callback cb, Args&&... args)
    {
        return detail::generic_query_can_fail(
            LI_NT(NtQuerySystemInformation), info_class, cb, std::forward<Args>(args)...);
    }

    template<class T>
    NT_FN update_info(SYSTEM_INFORMATION_CLASS info_class,
                      memory::unique_alloc<T>& info_buffer,
                      unsigned long&           buffer_size)
    {
        unsigned long size   = buffer_size;
        T*            buffer = info_buffer.get();

        const auto ntqsi  = LI_NT(NtQuerySystemInformation);
        auto       status = ntqsi(info_class, buffer, size, &size);

        if(status == STATUS_INFO_LENGTH_MISMATCH) {
            // add an extra page for more information
            size += 0x1000;

            buffer_size = 0;
            ret_on_err(info_buffer.allocate(size, PAGE_READWRITE));

            // set the buffer size
            buffer_size = size;

            status = ntqsi(info_class, info_buffer.get(), size, &size);
        }
        else
            buffer_size = size;

        return status;
    }

    template<class Callback, class... Args>
    NT_FN firmware_info(unsigned long table_signature,
                        unsigned long table_id,
                        Callback      cb,
                        Args&&... args)
    {
        SYSTEM_FIRMWARE_TABLE_INFORMATION info;
        info.ProviderSignature = table_signature;
        info.Action            = SystemFirmwareTableGet;
        info.TableID           = table_id;
        info.TableBufferLength = 0;

        const auto ntqsi = LI_NT(NtQuerySystemInformation);

        unsigned long return_len = 0;
        auto status = ntqsi(SystemFirmwareTableInformation, &info, 16u, &return_len);
        if(status == STATUS_INFO_LENGTH_MISMATCH || status == STATUS_BUFFER_TOO_SMALL) {
            memory::unique_alloc<SYSTEM_FIRMWARE_TABLE_INFORMATION> alloc;
            ret_on_err(alloc.allocate(return_len, PAGE_READWRITE));

            *alloc = info;

            status = ntqsi(
                SystemFirmwareTableInformation, alloc.get(), return_len, &return_len);
            if(NT_SUCCESS(status))
                cb(alloc->TableBuffer,
                   info.TableBufferLength,
                   std::forward<Args>(args)...);
        }

        return status;
    }

} // namespace ntw::system
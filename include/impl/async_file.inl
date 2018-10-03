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
#include "../io/async_file.hpp"

namespace ntw::io {

    template<class Handle, class Traits>
    NT_FN static async_file_traits<Handle>::open(void*&              handle,
                                                 OBJECT_ATTRIBUTES&  attributes,
                                                 const options_type& options,
                                                 unsigned long       disposition)
    {
        IO_STATUS_BLOCK status_block;
        return LI_NT(NtCreateFile)(&temp_handle,
                                   options._access,
                                   &attributes,
                                   &status_block,
                                   nullptr,
                                   options._attributes ? options._attributes
                                                       : FILE_ATTRIBUTE_NORMAL,
                                   options._share_access,
                                   disposition,
                                   options._options | FILE_NON_DIRECTORY_FILE,
                                   nullptr,
                                   0);
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::write(const void*   data,
                                                  unsigned long size,
                                                  std::int64_t  offset,
                                                  QueryData&    query) const noexcept
    {
        LARGE_INTEGER li_offset = make_large_int(offset);

        return LI_NT(NtWriteFile)(handle().get(),
                                  nullptr,
                                  [](void* context, IO_STATUS_BLOCK* iosb, unsigned long) {
                                      QueryData::on_completion(context);
                                  },
                                  query.reference(),
                                  &query.status_block(),
                                  const_cast<void*>(data),
                                  size,
                                  &li_offset,
                                  nullptr);
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::read(void*         buffer,
                                                 unsigned long size,
                                                 std::int64_t  offset,
                                                 QueryData&    query) const noexcept
    {
        LARGE_INTEGER li_offset = make_large_int(offset);
        return LI_NT(NtReadFile)(handle().get(),
                                 nullptr,
                                 [](void* context, IO_STATUS_BLOCK*, unsigned long) {
                                     QueryData::on_completion(context);
                                 },
                                 query.reference(),
                                 &query.status_block(),
                                 buffer,
                                 size,
                                 &li_offset,
                                 nullptr);
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::device_io_control(unsigned long control_code,
                                                              const void*   in_buffer,
                                                              unsigned long in_buffer_size,
                                                              void*         out_buffer,
                                                              unsigned long out_buffer_size,
                                                              QueryData& query) const noexcept
    {
        IO_STATUS_BLOCK status_block{ 0 };
        return LI_NT(NtDeviceIoControlFile)(
            handle().get(),
            nullptr,
            [](void* context, IO_STATUS_BLOCK*, unsigned long) {
                QueryData::on_completion(context);
            },
            query.reference(),
            &status_block,
            control_code,
            const_cast<void*>(in_buffer),
            in_buffer_size,
            out_buffer,
            out_buffer_size);
    }

    template<class Handle, class Traits>
    template<class InBuffer, class OutBuffer, class QueryData>
    NT_FN basic_async_file<Handle, Traits>::device_io_control(unsigned long   control_code,
                                                              const InBuffer& in_buffer,
                                                              OutBuffer&      out_buffer,
                                                              QueryData& query) const noexcept
    {
        return device_io_control(control_code,
                                 ::std::addressof(in_buffer),
                                 sizeof(InBuffer),
                                 ::std::addressof(out_buffer),
                                 sizeof(OutBuffer),
                                 query);
    }

} // namespace ntw::io

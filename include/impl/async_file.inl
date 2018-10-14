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
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::write(cbyte_span<unsigned long> data,
                                                  std::int64_t              offset,
                                                  QueryData& query) const noexcept
    {
        LARGE_INTEGER li_offset = make_large_int(offset);
        return LI_NT(NtWriteFile)(handle().get(),
                                  query.event(),
                                  detail::completion_routine<QueryData>(),
                                  query.reference(),
                                  &query.status_block(),
                                  const_cast<std::uint8_t*>(data.begin()),
                                  data.size(),
                                  &li_offset,
                                  nullptr);
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::read(byte_span<unsigned long> data,
                                                 std::int64_t              offset,
                                                 QueryData& query) const noexcept
    {
        LARGE_INTEGER li_offset = make_large_int(offset);
        return LI_NT(NtReadFile)(handle().get(),
                                 query.event(),
                                 detail::completion_routine<QueryData>(),
                                 query.reference(),
                                 &query.status_block(),
                                 data.begin(),
                                 data.size(),
                                 &li_offset,
                                 nullptr);
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN
    basic_async_file<Handle, Traits>::device_io_control(unsigned long control_code,
                                                        cbyte_span<unsigned long> input,
                                                        byte_span<unsigned long>  output,
                                                        QueryData& query) const noexcept
    {
        IO_STATUS_BLOCK status_block;
        return LI_NT(NtDeviceIoControlFile)(handle().get(),
                                            query.event(),
                                            detail::completion_routine<QueryData>(),
                                            query.reference(),
                                            &status_block,
                                            control_code,
                                            const_cast<std::uint8_t*>(input.begin()),
                                            input.size(),
                                            output.begin(),
                                            output.size());
    }

    template<class Handle, class Traits>
    template<class InBuffer, class OutBuffer, class QueryData>
    NT_FN basic_async_file<Handle, Traits>::device_io_control(unsigned long control_code,
                                                              const InBuffer& in_buffer,
                                                              OutBuffer&      out_buffer,
                                                              QueryData&      query) const
        noexcept
    {
        return device_io_control(control_code,
                                 { ::std::addressof(in_buffer), sizeof(InBuffer) },
                                 { ::std::addressof(out_buffer), sizeof(OutBuffer) },
                                 query);
    }

} // namespace ntw::io

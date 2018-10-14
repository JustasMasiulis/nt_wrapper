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
    template<class Fn, class QueryData>
    NT_FN basic_async_file<Handle, Traits>::_control(Fn                  fn,
                                                     ulong_t             control_code,
                                                     cbyte_span<ulong_t> input,
                                                     byte_span<ulong_t>  output,
                                                     QueryData& query) const noexcept
    {
        return fn(handle().get(),
                  query.event(),
                  detail::completion_routine<QueryData>(),
                  query.reference(),
                  &query.status_block(),
                  control_code,
                  const_cast<std::uint8_t*>(input.begin()),
                  input.size(),
                  output.begin(),
                  output.size());
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::write(cbyte_span<ulong_t> data,
                                                  std::int64_t        offset,
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
    NT_FN basic_async_file<Handle, Traits>::read(byte_span<ulong_t> data,
                                                 std::int64_t       offset,
                                                 QueryData&         query) const noexcept
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
    NT_FN basic_async_file<Handle, Traits>::device_io_control(ulong_t control_code,
                                                              cbyte_span<ulong_t> input,
                                                              byte_span<ulong_t>  output,
                                                              QueryData& query) const
        noexcept
    {
        return _control(LI_NT(NtDeviceIoControlFile), control_code, input, output, query);
    }

    template<class Handle, class Traits>
    template<class Input, class Output, class QueryData>
    NT_FN basic_async_file<Handle, Traits>::device_io_control(ulong_t      control_code,
                                                              const Input& input,
                                                              Output&      output,
                                                              QueryData&   query) const
        noexcept
    {
        return device_io_control(control_code,
                                 { ::std::addressof(input), sizeof(Input) },
                                 { ::std::addressof(output), sizeof(Output) },
                                 query);
    }

    template<class Handle, class Traits>
    template<class QueryData>
    NT_FN basic_async_file<Handle, Traits>::fs_control(ulong_t             control_code,
                                                       cbyte_span<ulong_t> input,
                                                       byte_span<ulong_t>  output,
                                                       QueryData& query) const noexcept
    {
        return _control(LI_NT(NtFsControlFile), control_code, input, output, query);
    }

    template<class Handle, class Traits>
    template<class Input, class Output, class QueryData>
    NT_FN basic_async_file<Handle, Traits>::fs_control(ulong_t      control_code,
                                                       const Input& input,
                                                       Output&      output,
                                                       QueryData&   query) const noexcept
    {
        return fs_control(control_code,
                          { ::std::addressof(input), sizeof(Input) },
                          { ::std::addressof(output), sizeof(Output) },
                          query);
    }

} // namespace ntw::io

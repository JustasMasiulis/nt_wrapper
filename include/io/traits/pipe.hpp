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
#include "../base_file.hpp"

namespace ntw::io::traits {

    template<class Handle, bool Synchronous>
    struct basic_pipe_traits {
        using handle_type  = Handle;
        using options_type = pipe_options;

        constexpr static auto options =
            options_type{}.share_all().full_access().byte_stream();

        NT_FN static open(void*&              handle,
                          OBJECT_ATTRIBUTES&  attributes,
                          const options_type& options,
                          unsigned long       disposition);
    };

    template<class Handle>
    using pipe_traits = basic_pipe_traits<Handle, true>;

    template<class Handle>
    using async_pipe_traits = basic_pipe_traits<Handle, false>;

    template<class Handle, bool Sync>
    NT_FN basic_pipe_traits<Handle, Sync>::open(void*&              handle,
                                                OBJECT_ATTRIBUTES&  attributes,
                                                const options_type& options,
                                                unsigned long       disposition)
    {
        IO_STATUS_BLOCK iosb;
        auto            timeout = make_large_int(options._timeout);
        return LI_NT(NtCreateNamedPipeFile)(
            &temp_handle,
            detail::synchronize_access<Sync>(options),
            &attributes,
            &iosb,
            options.data().share_access,
            disposition,
            detail::synchronize_options<Sync>(options),
            options.pipe_data().type,
            options.pipe_data().type & 1,
            Sync ? FILE_PIPE_QUEUE_OPERATION : FILE_PIPE_COMPLETE_OPERATION,
            options.pipe_data().instances_limit,
            options.pipe_data().inbound_qouta,
            options.pipe_data().outbound_qouta,
            &timeout);


    }; // namespace ntw::io::traits

} // namespace ntw::io::traits

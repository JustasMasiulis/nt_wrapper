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
#include "file.hpp"

namespace ntw::io {

    template<class Handle>
    struct pipe_traits {
        using handle_type  = Handle;
        using options_type = pipe_options;

        constexpr static auto options =
            ntw::io::pipe_options{}.share_all().full_access().sync().byte_stream();

        NT_FN static open(void*&              handle,
                          OBJECT_ATTRIBUTES&  attributes,
                          const options_type& options,
                          unsigned long       disposition);
    };

    template<class Handle, class Traits = pipe_traits<Handle>>
    class basic_pipe : public basic_file<Handle, Traits> {
        using base_type = basic_file<Handle, Traits>;

        NT_FN _fs_ctl(unsigned long code) const;

    public:
        NTW_INLINE basic_pipe()  = default;

        template<class ObjectHandle>
        NTW_INLINE basic_pipe(const ObjectHandle& handle) : base_type(handle)
        {}

		// TODO: add more pipe functionality for messages
        NT_FN listen() const;
        NT_FN disconnect() const;
    };

    using unique_pipe = basic_pipe<unique_handle>;
    using pipe_ref    = basic_pipe<handle_ref>;

} // namespace ntw::io

#include "../impl/async_file.inl"

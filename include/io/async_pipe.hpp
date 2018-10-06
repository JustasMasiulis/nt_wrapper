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
#include "traits/pipe.hpp"
#include "async_file.hpp"

namespace ntw::io {

    template<class Handle, class Traits = traits::async_pipe_traits<Handle>>
    class basic_async_pipe : public basic_async_file<Handle, Traits> {
        using base_type = basic_async_file<Handle, Traits>;

        template<class Query>
        NT_FN _fs_ctl(unsigned long code, Query& query) const;

    public:
        NTW_INLINE basic_async_pipe() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_async_pipe(const ObjectHandle& handle) : base_type(handle)
        {}

        // TODO: add more pipe functionality for messages
        template<class Query>
        NT_FN listen(Query& query) const;

        template<class Query>
        NT_FN disconnect(Query& query) const;
    };

    using unique_async_pipe = basic_async_pipe<unique_handle>;
    using async_pipe_ref    = basic_async_pipe<handle_ref>;

} // namespace ntw::io

#include "../impl/async_pipe.inl"

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
#include "traits/pipe.hpp"

namespace ntw::io {

    template<class Handle, class Traits = traits::pipe_traits<Handle>>
    class basic_pipe : public basic_file<Handle, Traits> {
        using base_type = basic_file<Handle, Traits>;

    public:
        NTW_INLINE basic_pipe() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_pipe(const ObjectHandle& handle) : base_type(handle)
        {}

        // TODO: add more pipe functionality for messages

        /// \brief Blocks untill a named pipe connection is made.
        /// \note ConnectNamedPipe equalient.
        NT_FN listen() const noexcept;

        /// \brief Disconnects the named pipe server from client.
        /// \note DisconnectNamedPipe qualient.
        NT_FN disconnect() const noexcept;
    };

    using unique_pipe = basic_pipe<unique_handle>;
    using pipe_ref    = basic_pipe<handle_ref>;

} // namespace ntw::io

#include "../impl/pipe.inl"

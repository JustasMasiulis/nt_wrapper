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
#include "traits/directory.hpp"

namespace ntw::io {

    template<class Handle, class Traits = traits::directory_traits<Handle>>
    class basic_directory : public detail::base_file<Traits> {
        using base_type = detail::base_file<Traits>;

    public:
        NTW_INLINE basic_directory() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_directory(const ObjectHandle& handle)
            : base_type(unwrap_handle(handle))
        {}

        /// \brief Enumerates contents of directory using NtQueryDirectoryFile API.
        /// \param callback Function that will be called for entry in a directory.
        /// \param args Arguments that will be passed to the callback function.
        /// \tparam StaticBufferSize the size of buffer that will be used for storing
        /// entries.
        template<std::size_t StaticBufferSize = 2048, class Callback, class... Args>
        NT_FN enum_contents(Callback&& callback, Args&&... args) const;

        /// \brief Enumerates contents of directory using NtQueryDirectoryFile API.
        /// \param buffer_begin The beginning of a buffer that will be used to store the
        /// file entries.
        /// \param buffer_end The end of a buffer that will be used to store
        /// the file entries.
        /// \param callback Function that will be called for entry in a
        /// directory.
        /// \param args Arguments that will be passed to the callback function.
        template<class Callback, class... Args>
        NT_FN enum_contents(byte_span<ulong_t> buffer, Callback cb, Args&&... args) const;
    };

    using unique_directory = basic_directory<unique_handle>;
    using directory_ref    = basic_directory<handle_ref>;

    template<class Callback, class... Args>
    NT_FN enum_directory(UNICODE_STRING name, Callback callback, Args&&... args);

} // namespace ntw::io

#include "../impl/directory.inl"

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
#include "base_file.hpp"

namespace ntw::io {

    template<class Handle>
    struct file_traits {
        using handle_type  = Handle;
        using options_type = file_options;

        constexpr static auto options = file_options{}.full_access().share_all();

        NT_FN static open(void*&              handle,
                          OBJECT_ATTRIBUTES&  attributes,
                          const options_type& options,
                          unsigned long       disposition);
    };

    template<class Handle, class Traits = file_traits<Handle>>
    class basic_file : public detail::base_file<Traits> {
        using base_type = base_file<Traits>;

    public:
        class disposer {
            std::wstring_view _path;

        public:
            NTW_INLINE disposer(std::wstring_view file_path) : _path(file_path) {}
            NTW_INLINE ~disposer() { static_cast<void>(unique_file::destroy(_path)); }
        };

        NTW_INLINE basic_file()  = default;
        NTW_INLINE ~basic_file() = default;

        template<class ObjectHandle>
        NTW_INLINE basic_file(const ObjectHandle& handle) : base_type(unwrap_handle(handle))
        {}

        NT_FN write(const void*    data,
                    unsigned long  size,
                    std::int64_t   offset  = 0,
                    unsigned long* written = nullptr) const noexcept;

        NT_FN
        read(void*          buffer,
             unsigned long  size,
             std::int64_t   offset = 0,
             unsigned long* read   = nullptr) const noexcept;

        /// \warning Likely to change as directory apis may be moved into their own
        /// abstraction
        template<class Callback, class... Args>
        NT_FN enum_contained_files(Callback callback, Args&&... args) const noexcept;

        NT_FN device_io_control(unsigned long  control_code,
                                const void*    in_buffer,
                                unsigned long  in_buffer_size,
                                void*          out_buffer,
                                unsigned long  out_buffer_size,
                                unsigned long* bytes_returned = nullptr) const noexcept;

        template<class InBuffer, class OutBuffer>
        NT_FN device_io_control(unsigned long   control_code,
                                const InBuffer& in_buffer,
                                OutBuffer&      out_buffer,
                                unsigned long*  bytes_returned = nullptr) const noexcept;
    };

    using unique_file = basic_file<unique_handle>;
    using file_ref    = basic_file<handle_ref>;

    template<class Callback, class... Args>
    NT_FN enum_directory(UNICODE_STRING name, Callback callback, Args&&... args);

} // namespace ntw::io

#include "../impl/file.inl"

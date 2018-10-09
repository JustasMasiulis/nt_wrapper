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
#include "handle.hpp"
#include "../util.hpp"

namespace ntw::obj {

    namespace detail {

        template<class Handle>
        class basic_section {
            Handle _handle;

        public:
            NTW_INLINE basic_section() noexcept = default;

            template<class ObjectHandle>
            NTW_INLINE basic_section(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            NT_FN create(std::int64_t  max_size,
                         unsigned long page_protection,
                         unsigned long alloc_attributes) noexcept
            {
                auto       li_max_size = make_large_int(max_size);
                void*      temp_handle = nullptr;
                const auto status = LI_NT(NtCreateSection)(&temp_handle,
                                                           SECTION_ALL_ACCESS,
                                                           nullptr,
                                                           &li_max_size,
                                                           page_protection,
                                                           alloc_attributes,
                                                           nullptr);

                if(NT_SUCCESS(status))
                    _handle.reset(temp_handle);

                return status;
            }

            template<class StringRef>
            NT_FN open(const StringRef& name,
                       unsigned long    access) noexcept
            {
                auto       uname = make_ustr(name);
                auto       attributes = make_attributes(&uname, OBJ_CASE_INSENSITIVE);
                void*      temp_handle = nullptr;
                const auto status = LI_NT(NtOpenSection)(&temp_handle,
                                                         access,
                                                         &attributes);

                if(NT_SUCCESS(status))
                    _handle.reset(temp_handle);

                return status;
            }

            NT_FN extend(std::int64_t new_size) noexcept
            {
                auto li_new_size = make_large_int(new_size);
                return LI_NT(NtExtendSection)(_handle.get(), &li_new_size);
            }

            template<class ProcessHandle, class Address>
            NT_FN map(const ProcessHandle& process,
                      Address&             base,
                      std::size_t          commit_size,
                      unsigned long        protection) noexcept
            {
                auto   offset = make_large_int(0);
                SIZE_T view_size;
                return LI_NT(NtMapViewOfSection)(_handle.get(),
                                                 unwrap_handle(process),
                                                 address_cast<void**>(&base),
                                                 0,
                                                 commit_size,
                                                 &offset,
                                                 &view_size,
                                                 ViewUnmap,
                                                 0,
                                                 protection);
            }

            template<class ProcessHandle, class Address>
            NT_FN static unmap(const ProcessHandle& process, Address base) noexcept
            {
                return LI_NT(NtUnmapViewOfSection)(unwrap_handle(process), address_cast<void*>(base));
            }
        };

    } // namespace detail

    using unique_section = detail::basic_section<unique_handle>;
    using section_ref    = detail::basic_section<handle_ref>;

} // namespace ntw::obj

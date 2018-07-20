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
            Handle _section;

        public:
            NTW_INLINE basic_section() noexcept = default;

            template<class ObjectHandle>
            NTW_INLINE basic_section(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _section; }
            NTW_INLINE const Handle& handle() const noexcept { return _section; }

            NT_FN create(std::int64_t  max_size,
                         unsigned long page_protection,
                         unsigned long alloc_attributes) noexcept
            {
                static_assert(sizeof(std::int64_t) == sizeof(LARGE_INTEGER));
                static_assert(alignof(std::int64_t) == alignof(LARGE_INTEGER));

                _section.reset();
                return LI_NT(NtCreateSection)(_section.addressof(),
                                              SECTION_ALL_ACCESS,
                                              nullptr,
                                              reinterpret_cast<LARGE_INTEGER*>(&max_size),
                                              page_protection,
                                              alloc_attributes,
                                              nullptr);
            }

            template<class ProcessHandle>
            NT_FN map(const ProcessHandle& process,
                      void**               base,
                      std::size_t          commit_size,
                      unsigned long        protection) noexcept
            {
                auto   offset = make_large_int(0);
                SIZE_T view_size;
                return LI_NT(NtMapViewOfSection)(_section.get(),
                                                 unwrap_handle(process),
                                                 base,
                                                 0,
                                                 commit_size,
                                                 &offset,
                                                 &view_size,
                                                 ViewUnmap,
                                                 0,
                                                 protection);
            }

            template<class ProcessHandle>
            NT_FN unmap(const ProcessHandle& process, void* base) noexcept
            {
                return LI_NT(NtUnmapViewOfSection)(unwrap_handle(process), base);
            }
        };

    } // namespace detail

    using unique_section = detail::basic_section<unique_handle>;
    using section_ref    = detail::basic_section<handle_ref>;

} // namespace ntw::obj

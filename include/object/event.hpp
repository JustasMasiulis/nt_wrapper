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
        class basic_event {
            Handle _handle;

        public:
            NTW_INLINE basic_event() noexcept = default;
            
            template<class ObjectHandle>
            NTW_INLINE basic_event(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            NT_FN create(EVENT_TYPE type) noexcept
            {
                return LI_NT(NtCreateEvent)(
                    _handle.addressof(), EVENT_ALL_ACCESS, nullptr, type, FALSE);
            }

            NT_FN wait(std::int64_t timeout) const noexcept
            {
                LARGE_INTEGER t;
                t.QuadPart = timeout;
                return LI_NT(NtWaitForSingleObject)(_handle.get(), FALSE, &t);
            }
        };

    } // namespace detail

    using unique_event = detail::basic_event<unique_handle>;
    using event_ref    = detail::basic_event<handle_ref>;

} // namespace ntw::obj

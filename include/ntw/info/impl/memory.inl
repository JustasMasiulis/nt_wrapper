/*
 * Copyright 2020 Justas Masiulis
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
#include "../memory.hpp"

namespace ntw::memory {

    NTW_INLINE std::uintptr_t basic_info::end() const noexcept { return base + size; }

    NTW_INLINE bool basic_info::is_commited() const noexcept
    {
        return state & MEM_COMMIT;
    }

    NTW_INLINE bool basic_info::is_reserved() const noexcept
    {
        return state & MEM_RESERVE;
    }

    NTW_INLINE bool basic_info::is_free() const noexcept { return state & MEM_FREE; }

    NTW_INLINE bool basic_info::is_mapped() const noexcept { return type & MEM_MAPPED; }

    NTW_INLINE bool basic_info::is_private() const noexcept { return type & MEM_PRIVATE; }

    NTW_INLINE bool basic_info::is_image() const noexcept { return type & MEM_IMAGE; }

    static_assert(sizeof(basic_info) == sizeof(MEMORY_BASIC_INFORMATION));

} // namespace ntw::memory
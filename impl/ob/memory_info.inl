#pragma once
#include "../../include/ntw/info/memory.hpp"

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
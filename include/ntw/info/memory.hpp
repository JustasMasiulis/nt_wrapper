#pragma once
#include "../result.hpp"
#include "../vm/protection.hpp"

namespace ntw::memory {

    struct basic_info {
        std::uintptr_t base;
        std::uintptr_t allocation_base;
        vm::protection allocation_protect;
        std::size_t    size;
        std::uint32_t  state;
        vm::protection protect;
        std::uint32_t  type;

        constexpr static MEMORY_INFORMATION_CLASS info_class = MemoryBasicInformation;
        using native_type                                    = MEMORY_BASIC_INFORMATION;

        /// \brief Returns end of the region
        NTW_INLINE std::uintptr_t end() const noexcept;

        /// \brief Checks whether state is equal to MEM_COMMIT flag.
        NTW_INLINE bool is_commited() const noexcept;

        /// \brief Checks whether state is equal to MEM_RESERVE flag.
        NTW_INLINE bool is_reserved() const noexcept;

        /// \brief Checks whether state is equal to MEM_FREE flag.
        NTW_INLINE bool is_free() const noexcept;

        /// \brief Checks whether type is equal to MEM_MAPPED flag.
        NTW_INLINE bool is_mapped() const noexcept;

        /// \brief Checks whether type is equal to MEM_PRIVATE flag.
        NTW_INLINE bool is_private() const noexcept;

        /// \brief Checks whether type is equal to MEM_IMAGE flag.
        NTW_INLINE bool is_image() const noexcept;
    };

} // namespace ntw::memory

#include "../../../impl/ob/memory_info.inl"
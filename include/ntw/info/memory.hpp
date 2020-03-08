#pragma once
#include "../result.hpp"

namespace ntw::memory {

    struct protection {
        std::uint32_t value = 0;

        NTW_INLINE constexpr protection() = default;

        NTW_INLINE constexpr protection(std::uint32_t value) noexcept;

        /// \brief Checks whether the values doesn't have PAGE_NOACCESS flag.
        NTW_INLINE constexpr bool accessible() const noexcept;

        /// \brief Returns whether the memory is readable.
        NTW_INLINE constexpr bool readable() const noexcept;

        /// \brief Returns whether the memory is writeable.
        NTW_INLINE constexpr bool writeable() const noexcept;

        /// \brief Returns whether the memory is executeable.
        NTW_INLINE constexpr bool executeable() const noexcept;

        /// \brief Returns whether PAGE_GUARD flag is set.
        NTW_INLINE constexpr bool guarded() const noexcept;

        /// \brief Returns whether PAGE_NOCACHE flag is set.
        NTW_INLINE constexpr bool non_cached() const noexcept;

        /// \brief Returns whether PAGE_WRITECOMBINE flag is set.
        NTW_INLINE constexpr bool write_combined() const noexcept;

        /// \brief Returns protection with PAGE_NOACCESS flag set.
        NTW_INLINE constexpr static protection no_access() noexcept;

        /// \brief Returns protection with PAGE_READONLY flag set.
        NTW_INLINE constexpr static protection read() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE flag set.
        NTW_INLINE constexpr static protection execute() noexcept;

        /// \brief Returns protection with PAGE_READWRITE flag set.
        NTW_INLINE constexpr static protection read_write() noexcept;

        /// \brief Returns protection with PAGE_WRITECOPY flag set.
        NTW_INLINE constexpr static protection read_writecopy() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE_READ flag set.
        NTW_INLINE constexpr static protection read_execute() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE_READWRITE flag set.
        NTW_INLINE constexpr static protection read_write_execute() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE_WRITECOPY flag set.
        NTW_INLINE constexpr static protection read_writecopy_execute() noexcept;

        /// \brief Enables PAGE_GUARD flag on this object.
        /// \returns *this
        NTW_INLINE constexpr protection& guard() noexcept;

        /// \brief Enables PAGE_NOCACHE flag on this object.
        /// \returns *this
        NTW_INLINE constexpr protection& disable_caching() noexcept;

        /// \brief Enables PAGE_NOCACHE flag on this object.
        /// \returns *this
        NTW_INLINE constexpr protection& combine_writes() noexcept;
    };

    struct basic_info {
        std::uintptr_t base;
        std::uintptr_t allocation_base;
        protection     allocation_protect;
        std::size_t    size;
        std::uint32_t  state;
        protection     protect;
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
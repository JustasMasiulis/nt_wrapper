#pragma once
#include "protection.hpp"

namespace ntw::vm {

    /// allocation_builder{process}
    ///     .resize(0x1000)
    ///     .protect(protection::read_write())
    ///     .commit_reserve();
    struct allocation_builder {
        const void*   address    = nullptr;
        std::size_t   zero_bytes = 0;
        std::uint32_t type       = 0;

        // allocation_builder(const allocation_builder&) = delete;
        // allocation_builder& operator=(const allocation_builder&) = delete;

        /// \brief Construct allocation_builder with nothing set.
        NTW_INLINE constexpr allocation_builder() noexcept = default;

        /// \brief Sets the amount of bytes to be zeroed
        /// \param amount The new amount of zero bytes.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& zero(std::size_t amount) noexcept;

        /// \brief Sets the preferred address for this allocation.
        /// \param address The new preferred allocation address.
        /// \returns *this
        template<class Address>
        NTW_INLINE constexpr allocation_builder& at(Address address) noexcept;

        /// \brief Enables MEM_COMMIT type flag and attempts to allocate memory.
        /// \returns *this
        template<class Process = void*>
        NTW_INLINE result<void*>
                   commit(std::size_t         size,
                          ntw::vm::protection prot,
                          const Process&      process = NtCurrentProcess()) const noexcept;

        /// \brief Enables MEM_RESERVE type flag and attempts to allocate memory.
        /// \returns *this
        template<class Process = void*>
        NTW_INLINE result<void*>
                   reserve(std::size_t         size,
                           ntw::vm::protection prot,
                           const Process&      process = NtCurrentProcess()) const noexcept;

        /// \brief Enables MEM_COMMIT and MEM_RESERVE type flags and attempts to allocate
        /// memory.
        /// \returns *this
        template<class Process = void*>
        NTW_INLINE result<void*>
                   commit_reserve(std::size_t         size,
                                  ntw::vm::protection prot = ntw::vm::protection::read_write(),
                                  const Process&      process = NtCurrentProcess()) const noexcept;

        /// \brief Enables MEM_RESET_UNDO type flag and attempts to allocate memory.
        /// \returns *this
        template<class Process = void*>
        NTW_INLINE result<void*>
                   reset_undo(std::size_t         size,
                              ntw::vm::protection prot,
                              const Process&      process = NtCurrentProcess()) const noexcept;


        /// \brief Enables MEM_RESET type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& reset() noexcept;

        /// \brief Enables MEM_TOP_DOWN type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& top_down() noexcept;

        /// \brief Enables MEM_WRITE_WATCH type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& write_watch() noexcept;

        /// \brief Enables MEM_PHYSICAL type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& physical() noexcept;

        /// \brief Enables MEM_WRITE_WATCH type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& rotate() noexcept;

        /// \brief Enables MEM_RESET_UNDO type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& large_pages() noexcept;

        // TODO NtAllocateVirtualMemoryEx equivalent of this
        /// \brief Enables MEM_REPLACE_PLACEHOLDER flag on this object.
        /// \note Valid only for NtAllocateVirtualMemoryEx
        /// \returns *this
        /// NTW_INLINE constexpr alloc_flags& replace_placeholder() noexcept;

        /// \brief Enables MEM_RESERVE_PLACEHOLDER flag on this object.
        /// \note Valid only for NtAllocateVirtualMemoryEx
        /// \returns *this
        /// NTW_INLINE constexpr alloc_flags& reserve_placeholder() noexcept;
    };

    NTW_INLINE constexpr allocation_builder allocate() noexcept;

} // namespace ntw::vm

#include "../../../impl/vm/allocation.inl"
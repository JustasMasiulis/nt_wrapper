#pragma once
#include "protection.hpp"
#include "../result.hpp"

namespace ntw::vm {

    /// allocate()
    ///     .at(address)
    ///     .commit_reserve(size, protection);
    class allocation_builder {
        const void*   _address = nullptr;
        std::size_t   _zero    = 0;
        std::uint32_t _type    = 0;

    public:
        /// \brief Construct allocation_builder with nothing set.
        NTW_INLINE constexpr allocation_builder() noexcept = default;

        /// \brief Sets the amount high-order address bits that must be zero in the base
        /// address. \param amount The new amount of zero bits. \note When amount is
        /// larger than 32, it becomes a bitmask. \returns *this
        NTW_INLINE constexpr allocation_builder& zero_bits(std::size_t amount) noexcept;

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

        /// \brief Enables MEM_TOP_DOWN type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& top_down() noexcept;

        /// \brief Enables MEM_WRITE_WATCH type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& write_watch() noexcept;

        /// \brief Enables MEM_PHYSICAL type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& physical() noexcept;

        /// \brief Enables MEM_ROTATE type flag.
        /// \returns *this
        NTW_INLINE constexpr allocation_builder& rotate() noexcept;

        /// \brief Enables MEM_LARGE_PAGES type flag.
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
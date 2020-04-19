#pragma once
#include "../../include/ntw/vm/allocation.hpp"
#include "../../include/ntw/detail/unwrap.hpp"
#include "../../include/ntw/detail/common.hpp"

namespace ntw::vm {

    namespace detail {

        NTW_INLINE result<void*> _allocate(const allocation_builder& builder,
                                           SIZE_T                    size,
                                           std::uint32_t             protection,
                                           void*                     process,
                                           std::uint32_t             flag) noexcept
        {
            void*    address = const_cast<void*>(builder.address);
            NTSTATUS result  = NTW_SYSCALL(NtAllocateVirtualMemory)(process,
                                                                   &address,
                                                                   builder.zero_bytes,
                                                                   &size,
                                                                   builder.type | flag,
                                                                   protection);
            return { result, address };
        }

    } // namespace detail

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::commit(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        return detail::_allocate(
            *this, size, prot.value, ::ntw::detail::unwrap(process), MEM_COMMIT);
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::reserve(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        return detail::_allocate(
            *this, size, prot.value, ::ntw::detail::unwrap(process), MEM_RESERVE);
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::commit_reserve(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        return detail::_allocate(*this,
                                 size,
                                 prot.value,
                                 ::ntw::detail::unwrap(process),
                                 MEM_COMMIT | MEM_RESERVE);
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::reset_undo(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        return detail::_allocate(
            *this, size, prot.value, ::ntw::detail::unwrap(process), MEM_RESET_UNDO);
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::zero(
        std::size_t new_ammount) noexcept
    {
        zero_bytes = new_ammount;
        return *this;
    }

    template<class Address>
    NTW_INLINE constexpr allocation_builder&
    allocation_builder::at(Address new_address) noexcept
    {
        address = reinterpret_cast<const void*>(new_address);
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::reset() noexcept
    {
        type |= MEM_RESET;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::top_down() noexcept
    {
        type |= MEM_TOP_DOWN;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::write_watch() noexcept
    {
        type |= MEM_WRITE_WATCH;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::physical() noexcept
    {
        type |= MEM_PHYSICAL;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::rotate() noexcept
    {
        type |= MEM_ROTATE;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::large_pages() noexcept
    {
        type |= MEM_LARGE_PAGES;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder allocate() noexcept { return {}; }

} // namespace ntw::vm
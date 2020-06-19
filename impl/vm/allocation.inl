#pragma once
#include "../../include/ntw/vm/allocation.hpp"
#include "../../include/ntw/detail/unwrap.hpp"
#include "../../include/ntw/detail/common.hpp"

namespace ntw::vm {

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::commit(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        SIZE_T   win_size = size;
        NTSTATUS result =
            NTW_SYSCALL(NtAllocateVirtualMemory)(::ntw::detail::unwrap(process),
                                                 &_address,
                                                 _zero,
                                                 &win_size,
                                                 _type | MEM_COMMIT,
                                                 prot.get());
        return { result, _address };
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::reserve(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        SIZE_T   win_size = size;
        NTSTATUS result =
            NTW_SYSCALL(NtAllocateVirtualMemory)(::ntw::detail::unwrap(process),
                                                 &_address,
                                                 _zero,
                                                 &win_size,
                                                 _type | MEM_RESERVE,
                                                 prot.get());
        return { result, _address };
    }

    template<class Process>
    NTW_INLINE result<void*> allocation_builder::commit_reserve(
        std::size_t size, protection prot, const Process& process) const noexcept
    {
        SIZE_T   win_size = size;
        NTSTATUS result =
            NTW_SYSCALL(NtAllocateVirtualMemory)(::ntw::detail::unwrap(process),
                                                 &_address,
                                                 _zero,
                                                 &win_size,
                                                 _type | MEM_COMMIT | MEM_RESERVE,
                                                 prot.get());
        return { result, _address };
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::zero_bits(
        std::size_t new_ammount) noexcept
    {
        _zero = new_ammount;
        return *this;
    }

    template<class Address>
    NTW_INLINE constexpr allocation_builder&
    allocation_builder::at(Address new_address) noexcept
    {
        _address = const_cast<void*>(reinterpret_cast<const void*>(new_address));
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::top_down() noexcept
    {
        _type |= MEM_TOP_DOWN;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::write_watch() noexcept
    {
        _type |= MEM_WRITE_WATCH;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::physical() noexcept
    {
        _type |= MEM_PHYSICAL;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::rotate() noexcept
    {
        _type |= MEM_ROTATE;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder& allocation_builder::large_pages() noexcept
    {
        _type |= MEM_LARGE_PAGES;
        return *this;
    }

    NTW_INLINE constexpr allocation_builder allocate() noexcept { return {}; }

} // namespace ntw::vm
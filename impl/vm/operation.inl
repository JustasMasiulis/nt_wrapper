#pragma once
#include "../../include/ntw/vm/operation.hpp"
#include "../../include/ntw/detail/unwrap.hpp"

namespace ntw::vm {

    template<class Address, class Process>
    NTW_INLINE status reset(Address        address,
                            std::size_t    size,
                            const Process& process) noexcept
    {
        SIZE_T s = size;
        void*  a = const_cast<void*>(reinterpret_cast<const void*>(address));
        return NTW_SYSCALL(NtAllocateVirtualMemory)(
            ::ntw::detail::unwrap(process), &a, 0, &s, MEM_RESET, PAGE_NOACCESS);
    }

    template<class Address, class Process>
    NTW_INLINE status undo_reset(Address        address,
                                 std::size_t    size,
                                 const Process& process) noexcept
    {
        SIZE_T s = size;
        void*  a = const_cast<void*>(reinterpret_cast<const void*>(address));
        return NTW_SYSCALL(NtAllocateVirtualMemory)(
            ::ntw::detail::unwrap(process), &a, 0, &s, MEM_RESET_UNDO, PAGE_NOACCESS);
    }

    template<class Address, class Process>
    NTW_INLINE status release(Address addr, const Process& process) noexcept
    {
        auto   casted = const_cast<void*>(reinterpret_cast<const void*>(addr));
        SIZE_T size   = 0;

        return NTW_SYSCALL(NtFreeVirtualMemory)(
            ::ntw::detail::unwrap(process), &casted, &size, MEM_RELEASE);
    }

    template<class Address, class Process>
    NTW_INLINE status decommit(Address        addr,
                               std::size_t    size,
                               const Process& process) noexcept
    {
        auto   casted   = const_cast<void*>(reinterpret_cast<const void*>(addr));
        SIZE_T win_size = size;

        return NTW_SYSCALL(NtFreeVirtualMemory)(
            ::ntw::detail::unwrap(process), &casted, &win_size, MEM_DECOMMIT);
    }

    template<class Address, class Process>
    NTW_INLINE status unmap(Address addr, const Process& process) noexcept
    {
        auto casted = const_cast<void*>(reinterpret_cast<const void*>(addr));
        return NTW_SYSCALL(NtUnmapViewOfSection)(::ntw::detail::unwrap(process), &casted);
    }

} // namespace ntw::vm
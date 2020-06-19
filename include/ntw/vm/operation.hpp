#pragma once
#include "../status.hpp"

namespace ntw::vm {

    template<class Address, class Process = void*>
    NTW_INLINE status reset(Address        address,
                            std::size_t    size,
                            const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status undo_reset(Address        address,
                                 std::size_t    size,
                                 const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status release(Address        addr,
                              const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status decommit(Address        addr,
                               std::size_t    size    = 0,
                               const Process& process = NtCurrentProcess()) noexcept;

    template<class Address, class Process = void*>
    NTW_INLINE status unmap(Address        addr,
                            const Process& process = NtCurrentProcess()) noexcept;

} // namespace ntw::vm

#include "../../../impl/vm/operation.inl"
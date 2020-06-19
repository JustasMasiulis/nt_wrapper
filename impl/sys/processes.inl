#include "../../include/ntw/sys/processes.hpp"

namespace ntw::sys {

    static_assert(sizeof(thread) == sizeof(SYSTEM_THREAD_INFORMATION));
    static_assert(sizeof(process) ==
                  (sizeof(SYSTEM_PROCESS_INFORMATION) - sizeof(thread)));

    template<class Range>
    NTW_INLINE ntw::result<process::range_type> processes(Range&&  buffer,
                                                          ulong_t* returned)
    {
        const auto  first  = detail::unfancy(detail::adl_begin(buffer));
        const auto  size   = static_cast<ulong_t>(detail::range_byte_size(buffer));
        ntw::status status = NTW_SYSCALL(NtQuerySystemInformation)(
            SystemProcessInformation, first, size, returned);

        return { status, { reinterpret_cast<process*>(first) } };
    }

    NTW_INLINE gsl::span<thread> process::threads() noexcept
    {
        return { reinterpret_cast<thread*>(this + 1), thread_count };
    }

    NTW_INLINE gsl::span<const thread> process::threads() const noexcept
    {
        return { reinterpret_cast<const thread*>(this + 1), thread_count };
    }

} // namespace ntw::sys
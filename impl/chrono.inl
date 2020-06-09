#include "../include/ntw/chrono.hpp"

namespace ntw {

    NTW_INLINE time_point clock::now() noexcept
    {
        return time_point{ duration{
            *reinterpret_cast<volatile std::int64_t*>(&USER_SHARED_DATA->SystemTime) } };
    }

    NTW_INLINE status sleep_for(duration dur) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = -dur.count();
        return NTW_SYSCALL(NtDelayExecution)(false, &interval);
    }

    NTW_INLINE status sleep_for(duration dur, alertable_t) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = -dur.count();
        return NTW_SYSCALL(NtDelayExecution)(true, &interval);
    }

    NTW_INLINE status sleep_until(time_point tp) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = tp.time_since_epoch().count();
        return NTW_SYSCALL(NtDelayExecution)(false, &interval);
    }

    NTW_INLINE status sleep_until(time_point tp, alertable_t) noexcept
    {
        LARGE_INTEGER interval;
        interval.QuadPart = tp.time_since_epoch().count();
        return NTW_SYSCALL(NtDelayExecution)(true, &interval);
    }

} // namespace ntw
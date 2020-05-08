#pragma once
#include <chrono>
#include <cstdint>
#include "detail/common.hpp"

namespace ntw {

    struct clock {
        using rep                       = std::int64_t;
        using period                    = std::ratio<1, 10000000>;
        using duration                  = std::chrono::duration<rep, period>;
        using time_point                = std::chrono::time_point<clock>;
        constexpr static bool is_steady = false; // not sure about this one to be honest

        NTW_INLINE static time_point now() noexcept
        {
            return time_point{ duration{ *reinterpret_cast<volatile std::int64_t*>(
                &USER_SHARED_DATA->SystemTime) } };
        }
    };

    using nanosecond_hundreds = std::ratio<1, 10000000>;
    using duration            = clock::duration;
    using time_point          = clock::time_point;

} // namespace ntw
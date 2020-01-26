#pragma once
#include <chrono>
#include <cstdint>

namespace ntw {

    using nanosecond_hundreds = std::ratio<1, 10000000>;
    using duration            = std::chrono::duration<std::int64_t, nanosecond_hundreds>;
    // TODO clock and time point type

} // namespace ntw
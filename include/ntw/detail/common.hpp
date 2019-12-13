#pragma once
#include "macros.hpp"
#include "../../../deps/GSL/include/gsl/span"
#include "../../../deps/phnt/phnt_windows.h"
#include "../../../deps/phnt/phnt.h"

namespace ntw {

    using byte_span  = gsl::span<std::uint8_t>;
    using cbyte_span = gsl::span<const std::uint8_t>;

    using ulong_t = unsigned long;

} // namespace ntw
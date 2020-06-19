#pragma once
#include "../../include/ntw/sys/pool_tags.hpp"

namespace ntw::sys {

    static_assert(sizeof(system_pooltag) == sizeof(SYSTEM_POOLTAG));

    template<class Range>
    NTW_INLINE ::ntw::result<gsl::span<system_pooltag>>
    pool_tags(Range&& buffer, ulong_t* returned) noexcept
    {
        const auto  first  = ::ntw::detail::unfancy(::ntw::detail::adl_begin(buffer));
        const auto  size   = static_cast<ulong_t>(::ntw::detail::range_byte_size(buffer));
        ntw::status status = NTW_SYSCALL(NtQuerySystemInformation)(
            SystemPoolTagInformation, first, size, returned);

        const auto info = reinterpret_cast<SYSTEM_POOLTAG_INFORMATION*>(first);
        return { status,
                 gsl::span<system_pooltag>{
                     reinterpret_cast<system_pooltag*>(info->TagInfo), info->Count } };
    }

} // namespace ntw::sys

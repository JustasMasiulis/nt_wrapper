#pragma once
#include "../result.hpp"

namespace ntw::sys {

    struct system_pooltag {
        std::uint32_t tag;
        struct {
            std::uint32_t allocations;
            std::uint32_t frees;
            std::size_t   used;
        } paged, non_paged;
    };

    /// \brief Queries pool tags in system use using SystemPoolTagInformation class.
    template<class Range>
    ntw::result<gsl::span<system_pooltag>> pool_tags(Range&&  buffer,
                                                     ulong_t* returned = nullptr) noexcept;

} // namespace ntw::sys

#include "../impl/sys/pool_tags.inl"
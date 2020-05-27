#pragma once
#define PHNT_VERSION PHNT_19H2
#include "macros.hpp"
#include "../../../deps/GSL/include/gsl/span"
#include "../../../deps/phnt/phnt_windows.h"
#include "../../../deps/phnt/phnt.h"
#include <iterator>

namespace ntw {

    using byte_span  = gsl::span<std::uint8_t>;
    using cbyte_span = gsl::span<const std::uint8_t>;

    using ulong_t = unsigned long;

    namespace detail {

        template<class Range>
        NTW_INLINE constexpr decltype(auto) adl_begin(Range&& r)
        {
            using std::begin;
            return begin(r);
        }

        template<class Range>
        NTW_INLINE constexpr decltype(auto) adl_end(Range&& r)
        {
            using std::end;
            return end(r);
        }

        template<class Range>
        NTW_INLINE constexpr auto range_byte_size(Range r)
        {
            using std::begin;
            using std::end;

            using traits     = std::iterator_traits<decltype(begin(r))>;
            using value_type = typename traits::value_type;

            static_assert(
                std::is_same<typename traits::iterator_category,
                             std::random_access_iterator_tag>::value,
                "iterators of ranges passed to nt_wrapper must be random access iterators");

            static_assert(
                std::is_trivially_copyable<value_type>::value,
                "Buffers passed to nt_wrapper must contain trivially copyable types");

            return (end(r) - begin(r)) * sizeof(value_type);
        }

        template<class It>
        NTW_INLINE constexpr auto unfancy(It iterator)
        {
            return ::std::addressof(*iterator);
        }

        struct set_info {};
        struct query_info {};

    } // namespace detail

} // namespace ntw
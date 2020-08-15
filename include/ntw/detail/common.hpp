/*
 * Copyright 2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#define PHNT_VERSION PHNT_19H1
#include "config.hpp"
#include <phnt_windows.h>
#include <phnt.h>
#include <iterator>
#include <cstdint>
#include <span>

namespace ntw {

    using byte_span  = std::span<std::uint8_t>;
    using cbyte_span = std::span<const std::uint8_t>;

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
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
    ntw::result<std::span<system_pooltag>>
    pool_tags(Range&& buffer, ulong_t* returned = nullptr) noexcept;

} // namespace ntw::sys

#include "impl/pool_tags.inl"
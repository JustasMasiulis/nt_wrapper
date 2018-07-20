/*
 * Copyright 2018 Justas Masiulis
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
#include "../detail/offset_iterator.hpp"
#include "../memory.hpp"

namespace ntw::system {

    class loaded_driver;

    namespace detail {

        struct loader_drivers_iterator_traits;

    }

    class loaded_driver {
        RTL_PROCESS_MODULE_INFORMATION_EX _me;

    public:
        NTW_INLINE void* section() const noexcept;
        NTW_INLINE void* mapped_base() const noexcept;
        NTW_INLINE void* image_base() const noexcept;
        NTW_INLINE void* default_base() const noexcept;
        NTW_INLINE std::uint32_t image_size() const noexcept;

        NTW_INLINE std::uint32_t flags() const noexcept;

        NTW_INLINE std::string_view name() const noexcept;
        NTW_INLINE std::string_view path() const noexcept;

        NTW_INLINE std::uint32_t checksum() const noexcept;
        NTW_INLINE std::uint32_t time_stamp() const noexcept;

        NTW_INLINE std::uint16_t load_order_index() const noexcept;
        NTW_INLINE std::uint16_t init_order_index() const noexcept;
        NTW_INLINE std::uint16_t load_count() const noexcept;

        NTW_INLINE std::uint16_t next_entry_offset() const noexcept;
    };

    class loaded_drivers {
        memory::unique_alloc<RTL_PROCESS_MODULE_INFORMATION_EX> _buffer;
        unsigned long                                           _buffer_size = 0;

    public:
        using value_type = RTL_PROCESS_MODULE_INFORMATION_EX;
        using iterator =
            ntw::detail::offset_iterator<detail::loader_drivers_iterator_traits>;

        NT_FN update() noexcept;

        NTW_INLINE iterator begin() noexcept;
        NTW_INLINE iterator end() noexcept;
    };

} // namespace ntw::system

#include "../impl/loaded_drivers.inl"

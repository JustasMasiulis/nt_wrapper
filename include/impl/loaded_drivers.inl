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
#include "../system/loaded_drivers.hpp"
#include "../system/info.hpp"

namespace ntw::system {

    namespace detail {

        struct loader_drivers_iterator_traits {
            using value_type = loaded_driver;
            NTW_INLINE static unsigned short next_entry_offset(value_type* val) noexcept
            {
                return val->next_entry_offset();
            }
        };

    } // namespace detail

    NTW_INLINE void* loaded_driver::section() const noexcept
    {
        return _me.BaseInfo.Section;
    }

    NTW_INLINE void* loaded_driver::mapped_base() const noexcept
    {
        return _me.BaseInfo.MappedBase;
    }

    NTW_INLINE void* loaded_driver::image_base() const noexcept
    {
        return _me.BaseInfo.ImageBase;
    }

    NTW_INLINE void* loaded_driver::default_base() const noexcept
    {
        return _me.DefaultBase;
    }

    NTW_INLINE std::uint32_t loaded_driver::image_size() const noexcept
    {
        return _me.BaseInfo.ImageSize;
    }

    NTW_INLINE std::uint32_t loaded_driver::flags() const noexcept
    {
        return _me.BaseInfo.Flags;
    }

    NTW_INLINE const char* loaded_driver::name() const noexcept
    {
        return path() + _me.BaseInfo.OffsetToFileName;
    }

    NTW_INLINE const char* loaded_driver::path() const noexcept
    {
        return reinterpret_cast<const char*>(_me.BaseInfo.FullPathName);
    }

    NTW_INLINE std::uint16_t loaded_driver::name_offset() const noexcept
    {
        return _me.BaseInfo.OffsetToFileName;
    }


    NTW_INLINE std::string_view loaded_driver::name_view() const noexcept
    {
        const auto first = name();
        auto       last  = first;
        while(*last)
            ++last;

        return std::string_view(first, last - first);
    }

    NTW_INLINE std::string_view loaded_driver::path_view() const noexcept
    {
        auto driver_name = name_view();
        return { driver_name.data() - _me.BaseInfo.OffsetToFileName,
                 driver_name.size() + _me.BaseInfo.OffsetToFileName };
    }

    NTW_INLINE std::uint32_t loaded_driver::checksum() const noexcept
    {
        return _me.ImageChecksum;
    }

    NTW_INLINE std::uint32_t loaded_driver::time_stamp() const noexcept
    {
        return _me.TimeDateStamp;
    }

    NTW_INLINE std::uint16_t loaded_driver::load_order_index() const noexcept
    {
        return _me.BaseInfo.LoadOrderIndex;
    }

    NTW_INLINE std::uint16_t loaded_driver::init_order_index() const noexcept
    {
        return _me.BaseInfo.InitOrderIndex;
    }

    NTW_INLINE std::uint16_t loaded_driver::load_count() const noexcept
    {
        return _me.BaseInfo.LoadCount;
    }

    NTW_INLINE std::uint16_t loaded_driver::next_entry_offset() const noexcept
    {
        return _me.NextOffset;
    }


    NT_FN loaded_drivers::update() noexcept
    {
        return system::update_info(SystemModuleInformationEx, _buffer, _buffer_size);
    }

    NTW_INLINE loaded_drivers::iterator loaded_drivers::begin() noexcept
    {
        return { std::launder(reinterpret_cast<loaded_driver*>(_buffer.get())) };
    }
    NTW_INLINE loaded_drivers::iterator loaded_drivers::end() noexcept { return {}; }

} // namespace ntw::system

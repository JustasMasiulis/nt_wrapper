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
#include "config.hpp"
#include <cstddef>
#include <iterator>

namespace ntw::detail {

    template<class T, bool CheckAfterAddition = false>
    class offset_iterator {
        T* _info = nullptr;

    public:
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using iterator_category = std::input_iterator_tag;

        NTW_INLINE constexpr offset_iterator() noexcept = default;

        NTW_INLINE constexpr offset_iterator(pointer info) noexcept : _info(info) {}

        NTW_INLINE reference operator*() const noexcept
        {
            return *reinterpret_cast<pointer>(_info);
        }

        NTW_INLINE pointer operator->() const noexcept
        {
            return reinterpret_cast<pointer>(_info);
        }

        NTW_INLINE offset_iterator& operator++() noexcept
        {
            const auto offset = _info->offset_to_next;
            _info = reinterpret_cast<pointer>(reinterpret_cast<char*>(_info) + offset);
            if((CheckAfterAddition && !_info->offset_to_next) ||
               (!CheckAfterAddition && !offset))
                _info = nullptr;
            return *this;
        }

        NTW_INLINE offset_iterator operator++(int) noexcept
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        NTW_INLINE bool _is_equal(const offset_iterator& other) const noexcept
        {
            return _info == other._info;
        }
    };

    template<class T, bool C>
    NTW_INLINE bool operator==(offset_iterator<T, C> lhs,
                               offset_iterator<T, C> rhs) noexcept
    {
        return lhs._is_equal(rhs);
    }

    template<class T, bool C>
    NTW_INLINE bool operator!=(offset_iterator<T, C> lhs,
                               offset_iterator<T, C> rhs) noexcept
    {
        return !lhs._is_equal(rhs);
    }

    template<class T, bool CheckAfterAddition = false>
    struct offset_iterator_range {
        using iterator_type = offset_iterator<T, CheckAfterAddition>;
        using value_type    = typename iterator_type::value_type;

        iterator_type value;

        offset_iterator_range(value_type* ptr) : value(ptr) {}

        iterator_type begin() const { return value; }
        iterator_type end() const { return {}; }
    };

} // namespace ntw::detail
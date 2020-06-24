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
#include "detail/common.hpp"
#include <string_view>
#include <cstdint>

namespace ntw {

    class unicode_string {
        UNICODE_STRING _value;

    public:
        using iterator       = wchar_t*;
        using const_iterator = const wchar_t*;

        using reverse_iterator       = std::reverse_iterator<wchar_t*>;
        using const_reverse_iterator = std::reverse_iterator<const wchar_t*>;


        /// \brief Constructrs an empty string
        NTW_INLINE constexpr unicode_string();

        /// \brief Constructs unicode_string when given a C string expression
        template<std::size_t N>
        NTW_INLINE constexpr unicode_string(const wchar_t (&str)[N]);

        /// \brief Constructrs unicode_string using the given string
        NTW_INLINE constexpr unicode_string(UNICODE_STRING str);

        /// \brief Constructs unicode_string out of wide string and its length
        /// \param len The length of given string in characters
        NTW_INLINE constexpr unicode_string(const wchar_t* str, std::uint16_t len);

        /// \brief Constructs unicode_string out of a view
        NTW_INLINE constexpr unicode_string(std::wstring_view view);

        /// \brief Returns the internal UNICODE_STRING
        NTW_INLINE UNICODE_STRING& get();

        /// \brief Returns the internal UNICODE_STRING
        NTW_INLINE const UNICODE_STRING& get() const;

        /// \brief Returns the beginning of buffer
        NTW_INLINE iterator begin();

        /// \brief Returns the beginning of buffer
        NTW_INLINE const_iterator begin() const;

        /// \brief Returns the beginning of buffer
        NTW_INLINE reverse_iterator rbegin();

        /// \brief Returns the beginning of buffer
        NTW_INLINE const_reverse_iterator rbegin() const;

        /// \brief Returns one past the end of buffer
        NTW_INLINE iterator end();

        /// \brief Returns one past the end of buffer
        NTW_INLINE const_iterator end() const;

        /// \brief Returns one past the end of buffer
        NTW_INLINE reverse_iterator rend();

        /// \brief Returns one past the end of buffer
        NTW_INLINE const_reverse_iterator rend() const;

        NTW_INLINE bool empty() const;

        /// \brief Returns the size of string in bytes
        NTW_INLINE std::uint16_t byte_size() const;

        /// \brief Returns the size of string in characters
        NTW_INLINE std::uint16_t size() const;

        /// \brief Returns a view of the internal string
        NTW_INLINE std::wstring_view view() const;
    };

} // namespace ntw

#include "impl/unicode_string.inl"
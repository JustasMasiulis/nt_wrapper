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
#include "detail/common.hpp"
#include <string_view>
#include <cstddef>

namespace ntw {

    template<std::size_t N>
    NTW_INLINE UNICODE_STRING make_ustr(const wchar_t (&str)[N]) noexcept;

    NTW_INLINE UNICODE_STRING make_ustr(std::wstring_view view) noexcept;

    NTW_INLINE UNICODE_STRING make_ustr(const UNICODE_STRING& us) noexcept;

    NTW_INLINE OBJECT_ATTRIBUTES make_attributes(UNICODE_STRING* object_name,
                                                 unsigned long   attributes,
                                                 void*           root_dir      = nullptr,
                                                 void*           security_desc = nullptr);

    NTW_INLINE LARGE_INTEGER make_large_int(std::int64_t i);

	template<class New, class Old>
	NTW_INLINE New address_cast(Old address) noexcept;

    template<class T>
    NTW_INLINE constexpr void assert_pointer_type() noexcept;

    template<class T>
    NTW_INLINE auto unwrap_handle(const T& handle) noexcept;

    template<class T>
    NTW_INLINE auto unwrap_handle_addressof(T& handle) noexcept;

} // namespace ntw

#include "impl/util.inl"

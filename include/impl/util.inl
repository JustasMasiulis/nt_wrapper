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
#include "../util.hpp"

namespace ntw {

    namespace detail {

        template<class, class = void>
        struct has_get : std::false_type {
        };

        template<class T>
        struct has_get<T, std::void_t<decltype(std::declval<T>().get())>>
            : std::true_type {
        };

        template<class, class = void>
        struct has_addressof : std::false_type {
        };

        template<class T>
        struct has_addressof<T, std::void_t<decltype(std::declval<T>().addressof())>>
            : std::true_type {
        };

    } // namespace detail

    template<std::size_t N>
    NTW_INLINE UNICODE_STRING make_ustr(const wchar_t (&str)[N]) noexcept
    {
        static_assert(N * 2 - 2 <= 0xffff); // unsigned short max
        UNICODE_STRING ustr; // size in bytes
        ustr.Length        = sizeof(wchar_t) * (N - 1);
        ustr.MaximumLength = sizeof(wchar_t) * N;
        ustr.Buffer        = const_cast<wchar_t*>(str);
        return ustr;
    }

    NTW_INLINE UNICODE_STRING make_ustr(std::wstring_view view) noexcept
    {
        UNICODE_STRING ustr; // size in bytes
        ustr.Length = ustr.MaximumLength =
            static_cast<unsigned short>(view.size() * sizeof(wchar_t));
        ustr.Buffer = const_cast<wchar_t*>(view.data());
        return ustr;
    }

    NTW_INLINE UNICODE_STRING make_ustr(const UNICODE_STRING& us) noexcept { return us; }

    NTW_INLINE OBJECT_ATTRIBUTES make_attributes(UNICODE_STRING* object_name,
                                                 unsigned long   attributes,
                                                 void*           root_dir,
                                                 void*           security_desc)
    {
        OBJECT_ATTRIBUTES attr;
        attr.Length                   = sizeof(OBJECT_ATTRIBUTES);
        attr.RootDirectory            = root_dir;
        attr.ObjectName               = object_name;
        attr.Attributes               = attributes;
        attr.SecurityDescriptor       = security_desc;
        attr.SecurityQualityOfService = nullptr;

        return attr;
    }

    NTW_INLINE LARGE_INTEGER make_large_int(std::int64_t i)
    {
        LARGE_INTEGER li;
        li.QuadPart = i;
        return li;
    }

    template<class T>
    NTW_INLINE constexpr void assert_pointer_type() noexcept
    {
        static_assert(alignof(T) == alignof(void*) && sizeof(T) == sizeof(void*),
                      "invalid pointer type");
    }

    template<class T>
    NTW_INLINE auto unwrap_handle(const T& handle) noexcept
    {
        if constexpr(std::is_pointer_v<T>)
            return handle;
        else if constexpr(detail::has_get<T>::value)
            return handle.get();
        else
            return handle.handle().get();
    }

    template<class T>
    NTW_INLINE auto unwrap_handle_addressof(T& handle) noexcept
    {
        if constexpr(std::is_pointer_v<T>)
            return &handle;
        else if constexpr(detail::has_addressof<T>::value)
            return handle.addressof();
        else
            return handle.handle().addressof();
    }

} // namespace ntw
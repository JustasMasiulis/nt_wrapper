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
        struct has_get : std::false_type {};

        template<class T>
        struct has_get<T, std::void_t<decltype(std::declval<T>().get())>>
            : std::true_type {};

        template<class, class = void>
        struct has_addressof : std::false_type {};

        template<class T>
        struct has_addressof<T, std::void_t<decltype(std::declval<T>().addressof())>>
            : std::true_type {};

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
    {}

    template<class New, class Old>
    NTW_INLINE New address_cast(Old address) noexcept
    {
        static_assert(std::is_pointer_v<Old> || std::is_unsigned_v<Old> ||
                          std::is_null_pointer_v<Old>,
                      "Address type must be either a pointer or unsigned");

        // yes this is cancerous and overkill but meh
        if constexpr(std::is_null_pointer_v<Old>)
            return New{ 0 };
        else if constexpr(std::is_pointer_v<Old>) {
            // if both types are pointers
            if constexpr(std::is_pointer_v<New>) {
                // casts to / from void are implicit or require static_cast
                if constexpr(std::is_void_v<std::remove_pointer_t<New>> ||
                             std::is_void_v<std::remove_pointer_t<Old>>)
                    return static_cast<New>(address);
                // else if pointer types do not match we should launder them
                else if constexpr(!std::is_same_v<New, Old>)
                    return std::launder(reinterpret_cast<New>(address));
                // both types are the same
                else
                    return address;
            }
            // else we just return what we casted
            else
                return reinterpret_cast<New>(address);
        }
        else {
            // if we need to cast to pointer use reinterpret
            if constexpr(std::is_pointer_v<New>)
                return reinterpret_cast<New>(address);
            // else use a simple static cast as reinterpret won't cast long -> int and
            // vice versa
            else
                return static_cast<New>(address);
        }
    }

    template<class T>
    NTW_INLINE auto unwrap_handle(const T& handle) noexcept
    {
        if constexpr(std::is_pointer_v<T> || std::is_null_pointer_v<T>)
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
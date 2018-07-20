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
#include "../detail/generic_query.hpp"

namespace ntw {

    namespace detail {
        template<template<class> class>
        struct basic_handle_traits;
    }

    using unique_handle =
        detail::unique_object<detail::basic_handle_traits<detail::unique_object>>;

    using handle_ref =
        detail::ref_object<detail::basic_handle_traits<detail::unique_object>>;

    namespace detail {

        template<template<class> class Base>
        struct basic_handle_traits {
            NT_FN
            duplicate(void*          process_to_duplicate_from,
                      unique_handle& handle,
                      ACCESS_MASK    desired_access,
                      unsigned long  attributes      = 0,
                      bool           same_access     = false,
                      bool           same_attributes = false) const noexcept
            {
                unsigned long options = 0;
                if(same_access)
                    options |= DUPLICATE_SAME_ACCESS;
                if(same_attributes)
                    options |= DUPLICATE_SAME_ATTRIBUTES;

                return LI_NT(NtDuplicateObject)(process_to_duplicate_from,
                                                base()->get(),
                                                NtCurrentProcess(),
                                                handle.addressof(),
                                                desired_access,
                                                attributes,
                                                options);
            }

            NT_FN close_remote(void* process_handle) const noexcept
            {
                return LI_NT(NtDuplicateObject)(process_handle,
                                                base()->get(),
                                                0,
                                                nullptr,
                                                0,
                                                0,
                                                DUPLICATE_CLOSE_SOURCE);
            }

            template<class Buffer, NTW_QUERY_BUFFER_REQUIREMENT>
            NT_FN info(OBJECT_INFORMATION_CLASS info_class,
                       Buffer&                  buffer,
                       unsigned long            size     = sizeof(Buffer),
                       unsigned long*           ret_size = nullptr) const noexcept
            {
                return LI_NT(NtQueryObject)(
                    base()->get(), info_class, buffer, size, ret_size);
            }

            template<class Callback, class... Args, NTW_QUERY_CALLBACK_REQUIREMENT>
            NT_FN info(OBJECT_INFORMATION_CLASS info_class,
                       Callback                 cb,
                       Args&&... args) const noexcept
            {
                NTW_IMPLEMENT_QUERY_CALLBACK
            }

            template<class Buffer>
            NT_FN set_info(OBJECT_INFORMATION_CLASS info_class,
                           Buffer&                  info,
                           unsigned long info_size = sizeof(Buffer)) const noexcept
            {
                return LI_NT(NtSetInformationObject)(
                    base()->get(), info_class, &info, info_size);
            }

            NT_FN set_security(SECURITY_INFORMATION info,
                               SECURITY_DESCRIPTOR& security) const noexcept
            {
                LI_NT(NtSetSecurityObject)(base()->get(), info, &security);
            }

        protected:
            using value_type = void;

            ~basic_handle_traits() = default;

            NTW_INLINE static void close(void* handle) noexcept
            {
                if(handle)
                    LI_NT(NtClose)(handle);
            }

        private:
            NTW_INLINE const Base<basic_handle_traits>* base() const noexcept
            {
                return static_cast<const Base<basic_handle_traits>*>(this);
            }
        };

    } // namespace detail
} // namespace ntw
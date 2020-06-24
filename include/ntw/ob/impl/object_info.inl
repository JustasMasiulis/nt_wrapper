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
#include "../object_info.hpp"

namespace ntw::object {

    template<class Object>
    NTW_INLINE status handle_flags_info::acquire(const Object& object)
    {
        return NTW_SYSCALL(NtQueryObject)(detail::unwrap(object),
                                          ObjectHandleFlagInformation,
                                          this,
                                          sizeof(handle_flags_info),
                                          nullptr);
    }

    template<class Object>
    NTW_INLINE status handle_flags_info::apply(const Object& object) const
    {
        return NTW_SYSCALL(NtSetInformationObject)(detail::unwrap(object),
                                                   ObjectHandleFlagInformation,
                                                   this,
                                                   sizeof(handle_flags_info));
    }

    NTW_INLINE ulong_t basic_info::attributes() const { return _info.Attributes; }

    NTW_INLINE ulong_t basic_info::access() const { return _info.GrantedAccess; }

    NTW_INLINE ulong_t basic_info::handle_count() const { return _info.HandleCount; }

    NTW_INLINE ulong_t basic_info::pointer_count() const { return _info.PointerCount; }

    NTW_INLINE ulong_t basic_info::paged_pool_charge() const
    {
        return _info.PagedPoolCharge;
    }

    NTW_INLINE ulong_t basic_info::non_page_pool_charge() const
    {
        return _info.NonPagedPoolCharge;
    }

    NTW_INLINE ulong_t basic_info::name_info_size() const { return _info.NameInfoSize; }

    NTW_INLINE ulong_t basic_info::type_info_size() const { return _info.TypeInfoSize; }

    NTW_INLINE ulong_t basic_info::security_desc_size() const
    {
        return _info.SecurityDescriptorSize;
    }

    NTW_INLINE std::int64_t basic_info::creation_time() const
    {
        return _info.CreationTime.QuadPart;
    }

    template<class Object>
    NTW_INLINE status basic_info::acquire(const Object& object)
    {
        return NTW_SYSCALL(NtQueryObject)(detail::unwrap(object),
                                          ObjectBasicInformation,
                                          &_info,
                                          sizeof(_info),
                                          nullptr);
    }

} // namespace ntw::object
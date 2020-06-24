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
#include "../unicode_string.hpp"
#include "../result.hpp"

namespace ntw::object {

    /// \brief Wraps OBJECT_HANDLE_FLAG_INFORMATION and operations with it
    struct handle_flags_info {
        /// \brief Whether the children will inherit this handle
        bool inherit;

        /// \brief Whether this handle is protected from closing
        bool protect_from_close;

        /// \brief Acquires OBJECT_HANDLE_FLAG_INFORMATION from given object
        template<class Object>
        NTW_INLINE status acquire(const Object& object);

        /// \brief Applies the OBJECT_HANDLE_FLAG_INFORMATION to the given object
        template<class Object>
        NTW_INLINE status apply(const Object& object) const;
    };

    /// \brief wraps OBJECT_BASIC_INFORMATION
    struct basic_info {
        OBJECT_BASIC_INFORMATION _info;

    public:
        /// \brief Returns OBJECT_BASIC_INFORMATION::Attributes
        /// \detail The attributes of the object
        NTW_INLINE ulong_t attributes() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::GrantedAccess
        /// \detail Mask that represents the granted access to the object
        NTW_INLINE ulong_t access() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::HandleCount
        /// \detail Specifies the number of handles to the object
        NTW_INLINE ulong_t handle_count() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::PointerCount
        /// \detail Specifies the number of pointers to the object
        NTW_INLINE ulong_t pointer_count() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::PagedPoolCharge
        NTW_INLINE ulong_t paged_pool_charge() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::NonPagedPoolCharge
        NTW_INLINE ulong_t non_page_pool_charge() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::NameInfoSize
        NTW_INLINE ulong_t name_info_size() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::TypeInfoSize
        NTW_INLINE ulong_t type_info_size() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::SecurityDescriptorSize
        NTW_INLINE ulong_t security_desc_size() const;

        /// \brief Returns OBJECT_BASIC_INFORMATION::CreationTime
        NTW_INLINE std::int64_t creation_time() const;

        /// \brief Acquires OBJECT_BASIC_INFORMATION from given object
        template<class Object>
        NTW_INLINE status acquire(const Object& object);
    };

    class type_info {
        unicode_string type_name() const;
        std::uint8_t   type_idx() const;

        ulong_t num_objects() const;
        ulong_t num_handles() const;
        bool    maintain_handle_count() const;

        ulong_t default_paged_pool_charge() const;
        ulong_t default_non_paged_pool_charge() const;

        ulong_t pool_type() const;
        ulong_t paged_pool_usage() const;
        ulong_t non_paged_pool_usage() const;
        ulong_t name_pool_usage() const;
        ulong_t handle_table_usage() const;

        GENERIC_MAPPING generic_mapping() const;
        ulong_t         valid_access_mask() const;
        ulong_t         invalid_attributes() const;
        bool            security_required() const;
    };

} // namespace ntw::object

#include "impl/object_info.inl"
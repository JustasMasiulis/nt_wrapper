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
#include "../detail/common.hpp"
#include "../detail/unwrap.hpp"
#include <cstdint>

namespace ntw::ob {

    /// \brief A type to build attribute options
    /// \tparam Derived the derived class whose "options()" member function
    ///         will be used to modify the state
    template<class Derived>
    struct basic_attribute_options {
        /// \brief Enables OBJ_INHERIT flag for the OBJECT_ATTRIBUTES
        /// \detail This handle can be inherited by children of the current process
        NTW_INLINE constexpr Derived& inherit();

        /// \brief Enables OBJ_PERMANENT flag for the OBJECT_ATTRIBUTES
        /// \detail Object will not be deleted when all open handles are closed.
        ///         ZwMakeTemporaryObject can be used to make object non-permanent
        NTW_INLINE constexpr Derived& permanent();

        /// \brief Enables OBJ_EXCLUSIVE flag for the OBJECT_ATTRIBUTES
        /// \detail No other process will be able to open a handle to this object
        NTW_INLINE constexpr Derived& exclusive();

        /// \brief Enables OBJ_CASE_INSENSITIVE flag for the OBJECT_ATTRIBUTES
        /// \detail Case insensitive comparison will be used for the ObjectName member
        NTW_INLINE constexpr Derived& case_insensitive();

        /// \brief Enables OBJ_OPENIF flag for the OBJECT_ATTRIBUTES
        /// \detail If object already exists STATUS_OBJECT_NAME_COLLISION will be returned
        NTW_INLINE constexpr Derived& open();

        /// \brief Enables OBJ_OPENLINK flag for the OBJECT_ATTRIBUTES
        /// \detail Opens symbolic link object itself instead of what it refers to
        NTW_INLINE constexpr Derived& open_symlink();

        /// \brief Enables OBJ_KERNEL_HANDLE flag for the OBJECT_ATTRIBUTES
        /// \detail Do not expose handle to current process if in kernel mode
        NTW_INLINE constexpr Derived& kernel_handle();

        /// \brief Enables OBJ_FORCE_ACCESS_CHECK flag for the OBJECT_ATTRIBUTES
        /// \detail Enforce all access checks if in kernel mode
        NTW_INLINE constexpr Derived& enforce_access_check();

        /// \brief Enables OBJ_IGNORE_IMPERSONATED_DEVICEMAP flag
        /// \detail Ignore the impersonated device map
        NTW_INLINE constexpr Derived& ignore_impersonated_devicemap();

        /// \brief Enables OBJ_DONT_REPARSE flag
        NTW_INLINE constexpr Derived& dont_reparse();
    };


    /// \brief A type to build attribute options separately
    struct attribute_options : basic_attribute_options<attribute_options> {
        unsigned long _options = 0;

        NTW_INLINE constexpr unsigned long& options();

        friend struct basic_attribute_options<attribute_options>;

    public:
        /// \brief Construct options with null value
        NTW_INLINE constexpr attribute_options() = default;

        /// \brief Returns the built options value
        NTW_INLINE constexpr unsigned long get() const;
    };


    /// \brief Class to build object attributes (besides the name)
    class attributes : public basic_attribute_options<attributes> {
        OBJECT_ATTRIBUTES _attributes;

        NTW_INLINE constexpr unsigned long& options();

        friend struct basic_attribute_options<attributes>;

    public:
        /// \brief Initializes attributes with its length and other fields set to 0
        NTW_INLINE constexpr attributes();

        /// \brief Returns the internal OBJECT_ATTRIBUTES structure
        NTW_INLINE constexpr OBJECT_ATTRIBUTES& get();

        /// \brief Returns the internal OBJECT_ATTRIBUTES structure
        NTW_INLINE constexpr const OBJECT_ATTRIBUTES& get() const;

        /// \brief Sets the RootDirectory field of OBJECT_ATTRIBUTES
        template<class Handle>
        NTW_INLINE constexpr attributes& parent(const Handle& parent);

        /// \brief Sets the SecurityDescriptor field of OBJECT_ATTRIBUTES
        NTW_INLINE constexpr attributes& security_desc(void* descriptor);

        /// \brief Sets the SecurityQualityOfService field of OBJECT_ATTRIBUTES
        NTW_INLINE constexpr attributes& security_quality(void* quality_of_service);

        /// \brief Sets the Attributes field of OBJECT_ATTRIBUTES
        NTW_INLINE constexpr attributes& options(attribute_options options);
    };

} // namespace ntw::ob

#include "impl/attributes.inl"
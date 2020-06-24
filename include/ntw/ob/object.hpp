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
#include "../detail/object_storage.hpp"
#include "../detail/unwrap.hpp"
#include "../result.hpp"
#include "../chrono.hpp"
#include "attributes.hpp"

namespace ntw::ob {

    template<class Storage>
    class basic_object : public Storage {
        template<class DOther>
        NTW_INLINE status _duplicate(void*                 process,
                                     basic_object<DOther>& out,
                                     unsigned long         access,
                                     unsigned long         attributes,
                                     unsigned long         options) const;

    public:
        /// \brief The type this object uses
        using storage_type = Storage;

        /// \brief Constructors and assignment operator are inherited from storage
        using storage_type::storage_type;
        using storage_type::operator=;

        /// \brief Returns the stored object
        using storage_type::get;

        /// \brief Releases ownership of stored object
        using storage_type::release;

        /// \brief Returns const reference tho the internal storage
        NTW_INLINE constexpr const storage_type& storage() const;

        /// \brief Returns reference tho the internal storage
        NTW_INLINE constexpr storage_type& storage();

        /// \brief Calls deleter on current object and sets it to new value
        NTW_INLINE constexpr void reset(void* new_handle) noexcept;

        /// \brief Same as reset(nullptr)
        NTW_INLINE constexpr void reset() noexcept;

        /// \brief Checks if stored value is not null
        NTW_INLINE constexpr explicit operator bool() const;

        /// \brief Makes object temporary
        /// \detail Object will be deleted once all handles to it are closed
        NTW_INLINE status make_temporary() const;

        /// \brief Makes object permanent
        /// \detail Object will not be deleted once all handles to it are closed
        NTW_INLINE status make_permanent() const;

        /// \brief Performs an infinite wait on the object
        NTW_INLINE status wait() const;

        /// \brief Performs an infinite wait on the object in an alertable state
        NTW_INLINE status wait(alertable_t) const;

        /// \brief Performs a wait on the object
        /// \param timeout The timeout of wait
        NTW_INLINE status wait_for(duration timeout) const;

        /// \brief Performs a wait on the object in an alertable state
        /// \param timeout The timeout of wait
        NTW_INLINE status wait_for(duration timeout, alertable_t) const;

        /// \brief Performs a wait on the object
        /// \param timeout The timeout of wait
        NTW_INLINE status wait_until(time_point time) const;

        /// \brief Performs a wait on the object in an alertable state
        /// \param timeout The timeout of wait
        NTW_INLINE status wait_until(time_point time, alertable_t) const;

        NTW_INLINE result_ref<unicode_string> name() const;
    };

    /// \brief Provides unique_ptr semantics for an object.
    using object = basic_object<detail::unique_object_storage>;

    /// \brief A reference type that does not own the object.
    using object_ref = basic_object<detail::object_ref_storage>;

} // namespace ntw::ob

#include "impl/object.inl"
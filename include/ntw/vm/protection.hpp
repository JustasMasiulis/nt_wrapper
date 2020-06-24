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
#include "../detail/config.hpp"
#include <cstdint>

namespace ntw::vm {

    struct protection {
        std::uint32_t value = 0;

        NTW_INLINE constexpr protection() = default;

        NTW_INLINE constexpr protection(std::uint32_t value) noexcept;

        NTW_INLINE constexpr std::uint32_t get() const noexcept;

        /// \brief Checks whether the values doesn't have PAGE_NOACCESS flag.
        NTW_INLINE constexpr bool accessible() const noexcept;

        /// \brief Returns whether the memory is readable.
        NTW_INLINE constexpr bool readable() const noexcept;

        /// \brief Returns whether the memory is writeable.
        NTW_INLINE constexpr bool writeable() const noexcept;

        /// \brief Returns whether the memory is executeable.
        NTW_INLINE constexpr bool executeable() const noexcept;

        /// \brief Returns whether PAGE_GUARD flag is set.
        NTW_INLINE constexpr bool guarded() const noexcept;

        /// \brief Returns whether PAGE_NOCACHE flag is set.
        NTW_INLINE constexpr bool non_cached() const noexcept;

        /// \brief Returns whether PAGE_WRITECOMBINE flag is set.
        NTW_INLINE constexpr bool write_combined() const noexcept;

        /// \brief Returns protection with PAGE_NOACCESS flag set.
        NTW_INLINE constexpr static protection no_access() noexcept;

        /// \brief Returns protection with PAGE_READONLY flag set.
        NTW_INLINE constexpr static protection read() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE flag set.
        NTW_INLINE constexpr static protection execute() noexcept;

        /// \brief Returns protection with PAGE_READWRITE flag set.
        NTW_INLINE constexpr static protection read_write() noexcept;

        /// \brief Returns protection with PAGE_WRITECOPY flag set.
        NTW_INLINE constexpr static protection read_writecopy() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE_READ flag set.
        NTW_INLINE constexpr static protection read_execute() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE_READWRITE flag set.
        NTW_INLINE constexpr static protection read_write_execute() noexcept;

        /// \brief Returns protection with PAGE_EXECUTE_WRITECOPY flag set.
        NTW_INLINE constexpr static protection read_writecopy_execute() noexcept;

        /// \brief Enables PAGE_GUARD flag on this object.
        /// \returns *this
        NTW_INLINE constexpr protection& guard() noexcept;

        /// \brief Enables PAGE_NOCACHE flag on this object.
        /// \returns *this
        NTW_INLINE constexpr protection& disable_caching() noexcept;

        /// \brief Enables PAGE_NOCACHE flag on this object.
        /// \returns *this
        NTW_INLINE constexpr protection& combine_writes() noexcept;
    };

} // namespace ntw::vm

#include "impl/protection.inl"
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

namespace ntw {

    template<class ObjectSpecific>
    class access_builder {
    protected:
        std::uint32_t _access = 0;

    public:
        NTW_INLINE constexpr access_builder() = default;

        /// \brief returns the stored status
        NTW_INLINE constexpr std::uint32_t get() const;

        /// \brief Enables GENERIC_READ flag
        NTW_INLINE constexpr ObjectSpecific& generic_read();

        /// \brief Enables GENERIC_WRITE flag
        NTW_INLINE constexpr ObjectSpecific& generic_write();

        /// \brief Enables GENERIC_EXECUTE flag
        NTW_INLINE constexpr ObjectSpecific& generic_execute();

        /// \brief Enables GENERIC_ALL flag
        NTW_INLINE constexpr ObjectSpecific& generic_all();

        /// \brief Enables ACCESS_SYSTEM_SECURITY flag
        NTW_INLINE constexpr ObjectSpecific& system_security();

        /// \brief Enables DELETE flag
        NTW_INLINE constexpr ObjectSpecific& destroy();

        /// \brief Enables READ_CONTROL flag
        NTW_INLINE constexpr ObjectSpecific& read_security();

        /// \brief Enables SYNCHRONIZE flag
        NTW_INLINE constexpr ObjectSpecific& synchronize();

        /// \brief Enables WRITE_DAC flag
        NTW_INLINE constexpr ObjectSpecific& write_dac();

        /// \brief Enables WRITE_OWNER flag
        NTW_INLINE constexpr ObjectSpecific& write_owner();

        /// \brief Enables MAXIMUM_ALLOWED flag
        NTW_INLINE constexpr ObjectSpecific& maximum_allowed();
    };

} // namespace ntw

#include "impl/access.inl"
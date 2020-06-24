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
#include "../result.hpp"
#include "../unicode_string.hpp"
#include "../io/registry_key.hpp"

namespace ntw::sys {

    struct driver {
        NTW_INLINE driver() = default;

        /// \brief Unloads driver unless release was called
        NTW_INLINE ~driver() noexcept;

        NTW_INLINE driver(driver&& other) noexcept;
        NTW_INLINE driver& operator=(driver&& other) noexcept;

        /// \brief Values used by CurrentControlSet\Services\<service name>\Start key.
        enum class start : ntw::ulong_t {
            boot,
            system,
            automatic,
            manual,
            disabled,
            manual_with_cleanup // manual with reg key that deletes itself on restart
        };

        /// \brief Values used by CurrentControlSet\Services\<service name>\ErrorControl
        /// key.
        enum class error_control : ntw::ulong_t { ignore, normal, severe, critical };

        /// \brief Values used by CurrentControlSet\Services\<service name>\Type key.
        /// \note There are more values than provided here
        enum class type : ntw::ulong_t {
            device_driver          = 1,
            file_system_driver     = 2,
            adapter_information    = 4,
            file_system_service    = 8,
            isolated_win32_service = 16,
            shared_win32_service   = 32
        };

        NTW_INLINE static result<driver> load(unicode_string path,
                                              start          start_type,
                                              error_control  error_control_type,
                                              type           driver_type) noexcept;

        /// \brief Unloads driver.
        /// \returns If driver is not loaded returns STATUS_NOT_FOUND otherwise return
        ///          value of NtUnloadDriver
        NTW_INLINE status unload() noexcept;

        /// \brief Prevents unload from being called in destructor
        NTW_INLINE void release() noexcept;

    private:
        unicode_string _service_path;
        wchar_t        _service_path_buffer[256];

        NTW_INLINE status _build_service_path(unicode_string file_path) noexcept;

        NTW_INLINE static status _init_service_fields(unicode_string            path,
                                                      const io::unique_reg_key& key,
                                                      start         start_type,
                                                      error_control error_control_type,
                                                      type          driver_type) noexcept;
    };

} // namespace ntw::sys

#include "impl/driver_loader.inl"
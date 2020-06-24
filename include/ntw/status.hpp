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
#include <cstdint>

namespace ntw {

    /// \brief Predefined NTSTATUS facilities
    enum class facility : std::int32_t {
        debugger            = 0x1,
        rpc_runtime         = 0x2,
        rpc_stubs           = 0x3,
        io_error_code       = 0x4,
        codclass_error_code = 0x6,
        ntwin32             = 0x7,
        ntcert              = 0x8,
        ntsspi              = 0x9,
        terminal_server     = 0xa,
        mui_error_code      = 0xb,
        usb_error_code      = 0x10,
        hid_error_code      = 0x11,
        firewire_error_code = 0x12,
        cluster_error_code  = 0x13,
        acpi_error_code     = 0x14,
        sxs_error_code      = 0x15,
        transaction         = 0x19,
        commonlog           = 0x1a,
        video               = 0x1b,
        filter_manager      = 0x1c,
        monitor             = 0x1d,
        graphics_kernel     = 0x1e,
        driver_framework    = 0x20,
        fve_error_code      = 0x21,
        fwp_error_code      = 0x22,
        ndis_error_code     = 0x23,
        tpm                 = 0x29,
        rtpm                = 0x2a,
        hypervisor          = 0x35,
        ipsec               = 0x36,
        virtualization      = 0x37,
        volmgr              = 0x38,
        bcd_error_code      = 0x39,
        win32k_ntuser       = 0x3e,
        win32k_ntgdi        = 0x3f,
        resume_key_filter   = 0x40,
        rdbss               = 0x41,
        bth_att             = 0x42,
        secureboot          = 0x43,
        audio_kernel        = 0x44,
        vsm                 = 0x45,
        volsnap             = 0x50,
        sdbus               = 0x51,
        shared_vhdx         = 0x5c,
        smb                 = 0x5d,
        interix             = 0x99,
        spaces              = 0xe7,
        security_core       = 0xe8,
        system_integrity    = 0xe9,
        licensing           = 0xea,
        platform_manifest   = 0xeb,
        app_exec            = 0xec,
        maximum_value       = 0xed,
    };

    /// \brief Predefined NTSTATUS severity codes
    enum class severity : std::int32_t {
        success     = 0,
        information = 1,
        warning     = 2,
        error       = 3
    };

    /// \brief Wraps NTSTATUS type and its helper functions
    class status {
        // | severity : 2 | customer : 1 | reserved : 1 | facility : 12 | code : 16 |
        std::int32_t _value = 0;

    public:
        /// \brief Constructs status with STATUS_SUCCESS
        NTW_INLINE constexpr status() = default;

        /// \brief Constructs status with given status code
        NTW_INLINE constexpr status(std::int32_t status);

        NTW_INLINE constexpr status& operator=(std::int32_t status);

        /// \brief Compares status codes
        NTW_INLINE constexpr bool operator==(std::int32_t status) const;

        /// \brief Wrapper around success() call
        NTW_INLINE constexpr explicit operator bool() const;

        /// \brief Implicit conversion to the system status type
        NTW_INLINE constexpr explicit operator std::int32_t() const;

        /// \brief Checks if status severity is not warning or error
        NTW_INLINE constexpr bool success() const;

        /// \brief Checks if status severity is information
        NTW_INLINE constexpr bool information() const;

        /// \brief Checks if status severity is warning
        NTW_INLINE constexpr bool warning() const;

        /// \brief Checks if status severity is error
        NTW_INLINE constexpr bool error() const;

        /// \brief Extracts the severity from status
        NTW_INLINE constexpr severity severity() const;

        /// \brief Extracts the facility from status
        NTW_INLINE constexpr facility facility() const;

        /// \brief Extracts the code from status
        NTW_INLINE constexpr std::int32_t code() const;

        /// \brief Returns the internal status value
        NTW_INLINE constexpr std::int32_t get() const;

        /// \brief Calls given function on success
        template<class Fn>
        NTW_INLINE constexpr status& and_then(Fn f);

        /// \brief Calls given function on !success
        template<class Fn>
        NTW_INLINE constexpr status& or_else(Fn f);
    };

} // namespace ntw

#include "impl/status.inl"
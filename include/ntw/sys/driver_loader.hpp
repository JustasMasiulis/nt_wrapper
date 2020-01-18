#pragma once
#include "../result.hpp"

namespace ntw::sys {

    class driver {
        unicode_string _service_path;

    public:
        /// \brief Values used by CurrentControlSet\Services\<service name>\Start key.
        enum class start { boot, system, automatic, manual, disabled };

        /// \brief Values used by CurrentControlSet\Services\<service name>\ErrorControl
        /// key.
        enum class error_control { ignore, normal, severe, critical };

        /// \brief Values used by CurrentControlSet\Services\<service name>\Type key.
        /// \note There are more values than provided here
        enum class type {
            device_driver          = 1,
            file_system_driver     = 2,
            adapter_information    = 4,
            file_system_service    = 8,
            isolated_win32_service = 16,
            shared_win32_service   = 32
        };

        NTW_INLINE static result<driver_loader> load(unicode_string path)
        {
            const auto ret = NTW_SYSCALL(NtLoadDriver)(&path.get());
            return ret;
        }
    };

} // namespace ntw::sys

#include "../../../impl/sys/driver_loader.inl"
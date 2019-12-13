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
    };

} // namespace ntw

#include "../impl/access.inl"

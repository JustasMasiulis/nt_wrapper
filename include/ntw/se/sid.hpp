#pragma once
#include "../detail/common.hpp"
#include <cstdint>

namespace ntw::se {

    template<std::size_t NumSubAuthorities>
    class static_sid {
        std::uint8_t                                 _revision             = SID_REVISION;
        std::uint8_t                                 _sub_auth_count       = 0;
        SID_IDENTIFIER_AUTHORITY                     _identifier_authority = { 0 };
        std::array<std::uint32_t, NumSubAuthorities> _sub_authorities      = { 0 };

        static_assert(NumSubAuthorities < SID_MAX_SUB_AUTHORITIES);

    public:
        /// \brief Initializes a null sid with all fields set to zero
        NTW_INLINE constexpr static_sid() = default;

        /// \brief Initializes sid with given authority and a variable number of sub
        /// authorities
        template<class... SubAuths>
        NTW_INLINE constexpr static_sid(SID_IDENTIFIER_AUTHORITY authority,
                                        SubAuths... subauthorities) noexcept;

        /// \brief Returns the number of sub authorities
        NTW_INLINE constexpr std::uint8_t size() const;

        /// \brief Returns the maximum number of sub authorities that can be stored
        NTW_INLINE constexpr std::uint8_t max_size() const;

        /// \brief Returns the indentifier authority of sid
        NTW_INLINE constexpr const SID_IDENTIFIER_AUTHORITY& identifier_authority() const;

        /// \brief Returns the indentifier authority of sid
        NTW_INLINE constexpr SID_IDENTIFIER_AUTHORITY& identifier_authority();

        /// \brief Returns a span of the initialized sub authorities
        NTW_INLINE constexpr gsl::span<std::uint32_t> sub_authorities();

        /// \brief Returns a span of the initialized sub authorities
        NTW_INLINE constexpr gsl::span<const std::uint32_t> sub_authorities() const;

        /// \brief Changes the amount of sub authorities stored
        NTW_INLINE constexpr void resize(std::size_t new_size);

        /// \brief Pushes back a new sub authority
        NTW_INLINE constexpr void push_back(std::uint32_t new_sub_auth);
    };

} // namespace ntw::se

#include "../impl/se/sid.inl"

namespace ntw::se::sid {

    namespace authority {

        using type = SID_IDENTIFIER_AUTHORITY;

        constexpr inline type null             = { 0, 0, 0, 0, 0, 0 };
        constexpr inline type world            = { 0, 0, 0, 0, 0, 1 };
        constexpr inline type local            = { 0, 0, 0, 0, 0, 2 };
        constexpr inline type creator          = { 0, 0, 0, 0, 0, 3 };
        constexpr inline type non_unique       = { 0, 0, 0, 0, 0, 4 };
        constexpr inline type resource_manager = { 0, 0, 0, 0, 0, 9 };

    } // namespace authority

    namespace rid {

        using type = std::uint32_t;

        constexpr inline type null        = 0;
        constexpr inline type world       = 0;
        constexpr inline type local       = 0;
        constexpr inline type local_logon = 1;

        constexpr inline type creator_owner = 0;
        constexpr inline type creator_group = 1;

        constexpr inline type creator_owner_server = 2;
        constexpr inline type creator_group_server = 3;

        constexpr inline type creator_owner_rights = 4;

    } // namespace rid

    namespace universal {

        using type = static_sid<1>;

        constexpr inline type null  = { authority::null, rid::null };
        constexpr inline type world = { authority::world, rid::world };
        constexpr inline type local = { authority::local, rid::local };

        constexpr inline type creator_owner = { authority::creator, rid::creator_owner };
        constexpr inline type creator_group = { authority::creator, rid::creator_group };

        constexpr inline type creator_owner_server = { authority::creator,
                                                       rid::creator_owner_server };
        constexpr inline type creator_group_server = { authority::creator,
                                                       rid::creator_group_server };

    } // namespace universal

} // namespace ntw::se::sid

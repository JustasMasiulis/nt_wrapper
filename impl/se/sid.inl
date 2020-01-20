#pragma once
#include "../../include/ntw/se/sid.hpp"

namespace ntw::se {

    template<std::size_t NSA>
    template<class... SubAuths>
    NTW_INLINE constexpr static_sid<NSA>::static_sid(SID_IDENTIFIER_AUTHORITY authority,
                                                     SubAuths... subauthorities) noexcept
        : _sub_auth_count(sizeof...(SubAuths))
        , _identifier_authority(authority)
        , _sub_authorities{ { subauthorities... } }
    {}

    template<std::size_t NSA>
    NTW_INLINE constexpr std::uint8_t static_sid<NSA>::size() const
    {
        return _sub_auth_count;
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr std::uint8_t static_sid<NSA>::max_size() const
    {
        return NSA;
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr const SID_IDENTIFIER_AUTHORITY&
    static_sid<NSA>::identifier_authority() const
    {
        return _identifier_authority;
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr SID_IDENTIFIER_AUTHORITY& static_sid<NSA>::identifier_authority()
    {
        return _identifier_authority;
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr gsl::span<const std::uint32_t>
    static_sid<NSA>::sub_authorities() const
    {
        return { _sub_authorities.data(), _sub_auth_count };
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr gsl::span<std::uint32_t> static_sid<NSA>::sub_authorities()
    {
        return { _sub_authorities.data(), _sub_auth_count };
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr void static_sid<NSA>::resize(std::size_t new_size)
    {
        _sub_auth_count = new_size;
    }

    template<std::size_t NSA>
    NTW_INLINE constexpr void static_sid<NSA>::push_back(std::uint32_t new_sub_auth)
    {
        _sub_authorities[_sub_auth_count++] = new_sub_auth;
    }

} // namespace ntw::se

#include "../../impl/se/sid.inl"
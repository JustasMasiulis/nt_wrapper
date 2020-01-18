#pragma once
#include "../include/ntw/access.hpp"

namespace ntw {

    template<class D>
    NTW_INLINE constexpr std::uint32_t access_builder<D>::get() const
    {
        return _access;
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::generic_read()
    {
        _access |= (1 << 31);
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::generic_write()
    {
        _access |= (1 << 30);
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::generic_execute()
    {
        _access |= (1 << 29);
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::generic_all()
    {
        _access |= (1 << 28);
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::system_security()
    {
        _access |= (1 << 24);
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::destroy()
    {
        _access |= DELETE;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::read_security()
    {
        _access |= READ_CONTROL;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::synchronize()
    {
        _access |= SYNCHRONIZE;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::write_dac()
    {
        _access |= WRITE_DAC;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& access_builder<D>::write_owner()
    {
        _access |= WRITE_OWNER;
        return static_cast<D&>(*this);
    }

} // namespace ntw
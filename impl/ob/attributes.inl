#pragma once
#include "../../include/ntw/ob/attributes.hpp"
#include "../../include/ntw/detail/unwrap.hpp"

namespace ntw::ob {

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::inherit()
    {
        // #define OBJ_INHERIT 0x00000002
        static_cast<D*>(this)->options() |= 0x00000002;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::permanent()
    {
        // #define OBJ_PERMANENT 0x00000010
        static_cast<D*>(this)->options() |= 0x00000010;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::exclusive()
    {
        // #define OBJ_EXCLUSIVE 0x00000020
        static_cast<D*>(this)->options() |= 0x00000020;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::case_insensitive()
    {
        // #define OBJ_CASE_INSENSITIVE 0x00000040
        static_cast<D*>(this)->options() |= 0x00000040;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::open()
    {
        // #define OBJ_OPENIF 0x00000080
        static_cast<D*>(this)->options() |= 0x00000080;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::open_symlink()
    {
        // #define OBJ_OPENLINK 0x00000100
        static_cast<D*>(this)->options() |= 0x00000100;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::kernel_handle()
    {
        // #define OBJ_KERNEL_HANDLE 0x00000200
        static_cast<D*>(this)->options() |= 0x00000200;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::enforce_access_check()
    {
        // #define OBJ_FORCE_ACCESS_CHECK 0x00000400
        static_cast<D*>(this)->options() |= 0x00000400;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::ignore_impersonated_devicemap()
    {
        // #define OBJ_IGNORE_IMPERSONATED_DEVICEMAP 0x00000800
        static_cast<D*>(this)->options() |= 0x00000800;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::dont_reparse()
    {
        // #define OBJ_DONT_REPARSE 0x00001000
        static_cast<D*>(this)->options() |= 0x00001000;
        return static_cast<D&>(*this);
    }


    NTW_INLINE constexpr unsigned long& attribute_options::options() { return _options; }

    NTW_INLINE constexpr unsigned long attribute_options::get() const { return _options; }


    NTW_INLINE constexpr unsigned long& attributes::options()
    {
        return _attributes.Attributes;
    }

    NTW_INLINE constexpr attributes::attributes()
        : _attributes{ sizeof(_attributes), nullptr, nullptr, 0, nullptr, nullptr }
    {}

    NTW_INLINE constexpr OBJECT_ATTRIBUTES& attributes::get() { return _attributes; }

    NTW_INLINE constexpr const OBJECT_ATTRIBUTES& attributes::get() const
    {
        return _attributes;
    }

    template<class Handle>
    NTW_INLINE constexpr attributes& attributes::parent(const Handle& parent)
    {
        _attributes.RootDirectory = ::ntw::detail::unwrap(parent);
        return *this;
    }

    NTW_INLINE constexpr attributes& attributes::security_desc(void* descriptor)
    {
        _attributes.SecurityDescriptor = descriptor;
        return *this;
    }

    NTW_INLINE constexpr attributes& attributes::security_quality(
        void* quality_of_service)
    {
        _attributes.SecurityQualityOfService = quality_of_service;
        return *this;
    }

    NTW_INLINE constexpr attributes& attributes::options(attribute_options options)
    {
        _attributes.Attributes = options.get();
        return *this;
    }

} // namespace ntw::ob

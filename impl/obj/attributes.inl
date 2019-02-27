#pragma once
#include "../../include/obj/attributes.hpp"

namespace ntw::obj {

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::inherit()
    {
        static_cast<D*>(this)->options() |= OBJ_INHERIT;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::permanent()
    {
        static_cast<D*>(this)->options() |= OBJ_PERMANENT;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::exclusive()
    {
        static_cast<D*>(this)->options() |= OBJ_EXCLUSIVE;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::case_insensitive()
    {
        static_cast<D*>(this)->options() |= OBJ_CASE_INSENSITIVE;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::open()
    {
        static_cast<D*>(this)->options() |= OBJ_OPENIF;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::open_symlink()
    {
        static_cast<D*>(this)->options() |= OBJ_OPENLINK;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::kernel_handle()
    {
        static_cast<D*>(this)->options() |= OBJ_KERNEL_HANDLE;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::enforce_access_check()
    {
        static_cast<D*>(this)->options() |= OBJ_FORCE_ACCESS_CHECK;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::ignore_impersonated_devicemap()
    {
        static_cast<D*>(this)->options() |= OBJ_IGNORE_IMPERSONATED_DEVICEMAP;
        return static_cast<D&>(*this);
    }

    template<class D>
    NTW_INLINE constexpr D& basic_attribute_options<D>::dont_reparse()
    {
        static_cast<D*>(this)->options() |= OBJ_DONT_REPARSE;
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
        _attributes.RootDirectory = detail::unwrap_handle(parent);
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

} // namespace ntw::obj

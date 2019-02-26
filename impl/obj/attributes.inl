#pragma once
#include "../../include/obj/attributes.hpp"

namespace ntw::obj {

    NTW_INLINE attributes::attributes()
    {
        _attributes.Length                   = sizeof(_attributes);
        _attributes.RootDirectory            = nullptr;
        _attributes.ObjectName               = nullptr;
        _attributes.Attributes               = 0;
        _attributes.SecurityDescriptor       = nullptr;
        _attributes.SecurityQualityOfService = nullptr;
    }

    OBJECT_ATTRIBUTES& attributes::get() { return _attributes; }

    const OBJECT_ATTRIBUTES& attributes::get() const { return _attributes; }

    template<class Handle>
    NTW_INLINE attributes& attributes::parent(const Handle& parent)
    {
        _attributes.RootDirectory = detail::unwrap_handle(parent);
        return *this;
    }

    NTW_INLINE attributes& attributes::security_desc(void* descriptor)
    {
        _attributes.SecurityDescriptor = descriptor;
        return *this;
    }

    NTW_INLINE attributes& attributes::security_quality(void* quality_of_service)
    {
        _attributes.SecurityQualityOfService = quality_of_service;
        return *this;
    }

    NTW_INLINE attributes& attributes::inherit()
    {
        _attributes.Attributes |= OBJ_INHERIT;
        return *this;
    }

    NTW_INLINE attributes& attributes::permanent()
    {
        _attributes.Attributes |= OBJ_PERMANENT;
        return *this;
    }

    NTW_INLINE attributes& attributes::exclusive()
    {
        _attributes.Attributes |= OBJ_EXCLUSIVE;
        return *this;
    }

    NTW_INLINE attributes& attributes::case_insensitive()
    {
        _attributes.Attributes |= OBJ_CASE_INSENSITIVE;
        return *this;
    }

    NTW_INLINE attributes& attributes::open()
    {
        _attributes.Attributes |= OBJ_OPENIF;
        return *this;
    }

    NTW_INLINE attributes& attributes::open_symlink()
    {
        _attributes.Attributes |= OBJ_OPENLINK;
        return *this;
    }

    NTW_INLINE attributes& attributes::kernel_handle()
    {
        _attributes.Attributes |= OBJ_KERNEL_HANDLE;
        return *this;
    }

    NTW_INLINE attributes& attributes::enforce_access_check()
    {
        _attributes.Attributes |= OBJ_FORCE_ACCESS_CHECK;
        return *this;
    }

    NTW_INLINE attributes& attributes::ignore_impersonated_devicemap()
    {
        _attributes.Attributes |= OBJ_IGNORE_IMPERSONATED_DEVICEMAP;
        return *this;
    }

    NTW_INLINE attributes& attributes::dont_reparse()
    {
        _attributes.Attributes |= OBJ_DONT_REPARSE;
        return *this;
    }

} // namespace ntw::obj

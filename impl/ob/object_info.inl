#pragma once
#include "../../include/ob/object_info.hpp"
#include "../../include/detail/generic_info_query.hpp"

namespace ntw::object {

    template<class Object>
    NTW_INLINE status handle_flags_info::acquire(const Object& object)
    {
        return NTW_SYSCALL(NtQueryObject)(detail::unwrap_handle(object),
                                          ObjectHandleFlagInformation,
                                          this,
                                          sizeof(handle_flags_info),
                                          nullptr);
    }

    template<class Object>
    NTW_INLINE status handle_flags_info::apply(const Object& object) const
    {
        return NTW_SYSCALL(NtSetInformationObject)(detail::unwrap_handle(object),
                                                   ObjectHandleFlagInformation,
                                                   this,
                                                   sizeof(handle_flags_info));
    }

    NTW_INLINE ulong_t basic_info::attributes() const { return _info.Attributes; }

    NTW_INLINE ulong_t basic_info::access() const { return _info.GrantedAccess; }
    
	NTW_INLINE ulong_t basic_info::handle_count() const { return _info.HandleCount; }
    
	NTW_INLINE ulong_t basic_info::pointer_count() const { return _info.PointerCount; }
    
	NTW_INLINE ulong_t basic_info::paged_pool_charge() const
    {
        return _info.PagedPoolCharge;
    }
    
	NTW_INLINE ulong_t basic_info::non_page_pool_charge() const
    {
        return _info.NonPagedPoolCharge;
    }
    
	NTW_INLINE ulong_t basic_info::name_info_size() const { return _info.NameInfoSize; }
    
	NTW_INLINE ulong_t basic_info::type_info_size() const { return _info.TypeInfoSize; }
    
	NTW_INLINE ulong_t basic_info::security_desc_size() const
    {
        return _info.SecurityDescriptorSize;
    }

    NTW_INLINE std::int64_t basic_info::creation_time() const
    {
        return _info.CreationTime.QuadPart;
    }

    template<class Object>
    NTW_INLINE status basic_info::acquire(const Object& object)
    {
        return NTW_SYSCALL(NtQueryObject)(detail::unwrap_handle(object),
                                          ObjectBasicInformation,
                                          &_info,
                                          sizeof(_info),
                                          nullptr);
    }

    template<class R>
    unicode_string name_info<R>::name()
    {
        return *static_cast<UNICODE_STRING*>(_res.get());
    }

    template<class R>
    template<class Object>
    NTW_INLINE status name_info<R>::acquire(const Object& object)
    {
        return detail::generic_info_query(_res, object, ObjectNameInformation, );
    }

} // namespace ntw::object
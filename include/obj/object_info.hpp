#pragma once
#include "../detail/resource_wrapper.hpp"
#include "../unicode_string.hpp"

namespace ntw::object {

    /// \brief Wraps OBJECT_HANDLE_FLAG_INFORMATION and operations with it
    struct handle_flags_info {
        bool inherit;
        bool protect_from_close;

        /// \brief Acquires OBJECT_HANDLE_FLAG_INFORMATION from given object
        template<class Object>
        NTW_INLINE status acquire(const Object& object);

        /// \brief Applies the OBJECT_HANDLE_FLAG_INFORMATION to the given object
        template<class Object>
        NTW_INLINE status apply(const Object& object) const;
    };

    /// \brief wraps OBJECT_BASIC_INFORMATION
    struct basic_info {
        OBJECT_BASIC_INFORMATION _info;

    public:
        NTW_INLINE ulong_t attributes() const;
        NTW_INLINE ulong_t access() const;
        NTW_INLINE ulong_t handle_count() const;
        NTW_INLINE ulong_t pointer_count() const;
        NTW_INLINE ulong_t paged_pool_charge() const;
        NTW_INLINE ulong_t non_page_pool_charge() const;
        NTW_INLINE ulong_t name_info_size() const;
        NTW_INLINE ulong_t type_info_size() const;
        NTW_INLINE ulong_t security_desc_size() const;
        NTW_INLINE std::int64_t creation_time() const;

        template<class Object>
        NTW_INLINE status acquire(const Object& object);
    };

    template<class Resource = ntw::stack_buffer<sizeof(UNICODE_STRING) + MAX_PATH>>
    class name_info {
        detail::resource_wrapper<Resource> _res;

    public:
        unicode_string name();

        template<class Object>
        NTW_INLINE status acquire(const Object& object);
    };

    template<class Resource =
                 ntw::stack_buffer<sizeof(OBJECT_TYPE_INFORMATION) + MAX_PATH>>
    struct type_info {
        detail::resource_wrapper<Resource> _res;

        unicode_string type_name() const;
        std::uint8_t   type_idx() const;

        ulong_t num_objects() const;
        ulong_t num_handles() const;
        bool    maintain_handle_count() const;

        ulong_t default_paged_pool_charge() const;
        ulong_t default_non_paged_pool_charge() const;

        ulong_t pool_type() const;
        ulong_t paged_pool_usage() const;
        ulong_t non_paged_pool_usage() const;
        ulong_t name_pool_usage() const;
        ulong_t handle_table_usage() const;


        GENERIC_MAPPING generic_mapping() const;
        ulong_t         valid_access_mask() const;
        ulong_t         invalid_attributes() const;
        bool            security_required() const;
    };

} // namespace ntw::object

#include "../../include/obj/object_info.hpp"
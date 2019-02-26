#pragma once
#include "../detail/common.hpp"
#include "../detail/unwrap_handle.hpp"

namespace ntw::obj {

    /// \brief Class to build object attributes (besides the name)
    class attributes {
        OBJECT_ATTRIBUTES _attributes;

    public:
        /// \brief Initializes attributes with its length and other fields set to 0
        NTW_INLINE attributes();

        /// \brief Returns the internal OBJECT_ATTRIBUTES structure
        OBJECT_ATTRIBUTES& get();

        /// \brief Returns the internal OBJECT_ATTRIBUTES structure
        const OBJECT_ATTRIBUTES& get() const;

        /// \brief Sets the RootDirectory field of OBJECT_ATTRIBUTES
        template<class Handle>
        NTW_INLINE attributes& parent(const Handle& parent);

        /// \brief Sets the SecurityDescriptor field of OBJECT_ATTRIBUTES
        NTW_INLINE attributes& security_desc(void* descriptor);

        /// \brief Sets the SecurityQualityOfService field of OBJECT_ATTRIBUTES
        NTW_INLINE attributes& security_quality(void* quality_of_service);

        /// \brief Enables OBJ_INHERIT flag for the OBJECT_ATTRIBUTES
        /// \detail This handle can be inherited by children of the current process
        NTW_INLINE attributes& inherit();

        /// \brief Enables OBJ_PERMANENT flag for the OBJECT_ATTRIBUTES
        /// \detail Object will not be deleted when all open handles are closed.
        ///         ZwMakeTemporaryObject can be used to make object non-permanent
        NTW_INLINE attributes& permanent();

        /// \brief Enables OBJ_EXCLUSIVE flag for the OBJECT_ATTRIBUTES
        /// \detail No other process will be able to open a handle to this object
        NTW_INLINE attributes& exclusive();

        /// \brief Enables OBJ_CASE_INSENSITIVE flag for the OBJECT_ATTRIBUTES
        /// \detail Case insensitive comparison will be used for the ObjectName member
        NTW_INLINE attributes& case_insensitive();

        /// \brief Enables OBJ_OPENIF flag for the OBJECT_ATTRIBUTES
        /// \detail If object already exists STATUS_OBJECT_NAME_COLLISION will be returned
        NTW_INLINE attributes& open();

        /// \brief Enables OBJ_OPENLINK flag for the OBJECT_ATTRIBUTES
        /// \detail Opens symbolic link object itself instead of what it refers to
        NTW_INLINE attributes& open_symlink();

        /// \brief Enables OBJ_KERNEL_HANDLE flag for the OBJECT_ATTRIBUTES
        /// \detail Do not expose handle to current process if in kernel mode
        NTW_INLINE attributes& kernel_handle();

        /// \brief Enables OBJ_FORCE_ACCESS_CHECK flag for the OBJECT_ATTRIBUTES
        /// \detail Enforce all access checks if in kernel mode
        NTW_INLINE attributes& enforce_access_check();

        /// \brief Enables OBJ_IGNORE_IMPERSONATED_DEVICEMAP flag
        /// \detail Ignore the impersonated device map
        NTW_INLINE attributes& ignore_impersonated_devicemap();

        /// \brief Enables OBJ_DONT_REPARSE flag
        NTW_INLINE attributes& dont_reparse();
    };

} // namespace ntw::obj

#include "../impl/obj/attributes.inl"

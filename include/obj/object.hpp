#pragma once
#include "../detail/unwrap.hpp"
#include "../status.hpp"
#include "attributes.hpp"
#include "../deps/GSL/include/gsl/span"

namespace ntw::obj {

    struct same_access_t {
        explicit same_access_t() = default;
    };

    struct same_attributes_t {
        explicit same_attributes_t() = default;
    };

    struct alertable_t {
        explicit alertable_t() = default;
    };

    constexpr inline same_access_t     same_access;
    constexpr inline same_attributes_t same_attributes;
    constexpr inline alertable_t       alertable;

    template<class Deleter>
    class basic_object {
        void* _value = nullptr;

        template<class DOther>
        NTW_INLINE status _duplicate(void*                 process,
                                     basic_object<DOther>& out,
                                     unsigned long         access,
                                     unsigned long         attributes,
                                     unsigned long         options) const;

    public:
        using nanosecond_hundreds =
            std::chrono::duration<std::int64_t, std::ratio<1, 10000000>>;

        /// \brief Calls deleter on the stored value
        NTW_INLINE ~basic_object();

        /// \brief Construct object with null value
        NTW_INLINE constexpr basic_object() = default;

        /// \brief Constructs object with given value
        NTW_INLINE constexpr basic_object(void* handle);

        /// \brief Constructs object with given value. Other object gets set to null
        NTW_INLINE basic_object(basic_object&& other);

        /// \brief Assigns another object to current object. Other objects gets set to
        ///		   null
        NTW_INLINE basic_object& operator=(basic_object&& other);

        /// \brief Returns stored value
        NTW_INLINE constexpr void* get() const;

        /// \brief Returns address of stored value
        NTW_INLINE constexpr void** addressof();

        /// \brief Calls deleter on current object and sets it to new value
        NTW_INLINE void reset(void* new_handle) noexcept;

        /// \brief Same as reset(nullptr)
        NTW_INLINE void reset() noexcept;

        /// \brief Releases ownership
        NTW_INLINE constexpr void* release() noexcept;

        /// \brief Checks if stored value is not null
        NTW_INLINE constexpr explicit operator bool() const;

        /// \brief Duplicates object
        /// \param process The process out of which to duplicate this object
        /// \param out The duplicated handle
        /// \param access The access to request for
        /// \param attr The attributes to ask for
        template<class Process, class DOther, class Access>
        NTW_INLINE status duplicate(const Process&        process,
                                    basic_object<DOther>& out,
                                    Access                access,
                                    attribute_options     attr) const;

        /// \brief Duplicates object with same access
        /// \param process The process out of which to duplicate this object
        /// \param out The duplicated handle
        /// \param attr The attributes to ask for
        template<class Process, class DOther>
        NTW_INLINE status duplicate(const Process&        process,
                                    basic_object<DOther>& out,
                                    same_access_t,
                                    attribute_options attr) const;

        /// \brief Duplicates object with same attributes
        /// \param process The process out of which to duplicate this object
        /// \param out The duplicated handle
        /// \param access The access to request for
        template<class Process, class DOther, class Access>
        NTW_INLINE status duplicate(const Process&        process,
                                    basic_object<DOther>& out,
                                    Access                access,
                                    same_attributes_t) const;

        /// \brief Duplicates object with same attributes and access
        /// \param process The process out of which to duplicate this object
        /// \param out The duplicated handle
        template<class Process, class DOther>
        NTW_INLINE status duplicate(const Process&        process,
                                    basic_object<DOther>& out,
                                    same_access_t,
                                    same_attributes_t) const;

        /// \brief Calls duplicate with process parameter bound to current process
        template<class DOther, class Access, class Attributes>
        NTW_INLINE status duplicate(basic_object<DOther>& out,
                                    Access                access,
                                    Attributes            attr) const;

        /// \brief Closes the handle in a remote process using NtDuplicateObject with
        ///        DUPLICATE_CLOSE_SOURCE flag
        /// \param process The process in which to close the handle
        template<class Process>
        NTW_INLINE status close_remote(const Process& process) const;

        /// \brief Makes object temporary
        /// \detail Object will be deleted once all handles to it are closed
        NTW_INLINE status make_temporary() const;

        /// \brief Makes object permanent
        /// \detail Object will not be deleted once all handles to it are closed
        NTW_INLINE status make_permanent() const;

        /// \brief Performs an infinite wait on the object
        NTW_INLINE status wait() const;

        /// \brief Performs an infinite wait on the object in an alertable state
        NTW_INLINE status wait(alertable_t) const;

        /// \brief Performs a wait on the object
        /// \param timeout The timeout of wait
        NTW_INLINE status wait_for(nanosecond_hundreds timeout) const;

        /// \brief Performs a wait on the object in an alertable state
        /// \param timeout The timeout of wait
        NTW_INLINE status wait_for(nanosecond_hundreds timeout, alertable_t) const;

        template<class T, std::ptrdiff_t Extent>
        NTW_INLINE status info(OBJECT_INFORMATION_CLASS info_type,
                               gsl::span<T, Extent>     data,
                               ulong_t*                 return_size = nullptr) const;
    };

} // namespace ntw::obj

#include "../../impl/obj/object.inl"

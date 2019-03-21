#pragma once
#include "../detail/unwrap.hpp"
#include "../status.hpp"
#include "attributes.hpp"

namespace ntw::ob {

    struct same_access_t {
        explicit constexpr same_access_t() = default;
    };

    struct same_attributes_t {
        explicit constexpr same_attributes_t() = default;
    };

    struct alertable_t {
        explicit constexpr alertable_t() = default;
    };

    constexpr inline same_access_t     same_access;
    constexpr inline same_attributes_t same_attributes;
    constexpr inline alertable_t       alertable;

    template<class Storage>
    class basic_object : public Storage {
        template<class DOther>
        NTW_INLINE status _duplicate(void*                 process,
                                     basic_object<DOther>& out,
                                     unsigned long         access,
                                     unsigned long         attributes,
                                     unsigned long         options) const;

    public:
        /// \brief The type this object uses
        using storage_type = Storage;

        /// \brief The time format used by windows
        using nanosecond_hundreds =
            std::chrono::duration<std::int64_t, std::ratio<1, 10000000>>;

        /// \brief Constructors are inherited from storage
        using storage_type::storage_type;

        /// \brief Returns the stored object
        using storage_type::get;

        /// \brief Releases ownership of stored object
        using storage_type::release;

        /// Copy assigns storage in this from other
        template<class SO>
        NTW_INLINE constexpr basic_object& operator=(const basic_object<SO>& other);

        /// Move assigns storage in this from other
        template<class SO>
        NTW_INLINE constexpr basic_object& operator=(basic_object<SO>&& other);

        /// \brief Returns const reference tho the internal storage
        NTW_INLINE constexpr const storage_type& storage() const;

        /// \brief Returns reference tho the internal storage
        NTW_INLINE constexpr storage_type& storage();

        /// \brief Calls deleter on current object and sets it to new value
        NTW_INLINE constexpr void reset(void* new_handle) noexcept;

        /// \brief Same as reset(nullptr)
        NTW_INLINE constexpr void reset() noexcept;

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
    };

    namespace detail {

        class object_ref_storage {
            void* _value = nullptr;

        public:
            /// \brief Constructs empty storage
            NTW_INLINE constexpr object_ref_storage() noexcept = default;

            /// \brief Constructs empty storage
            NTW_INLINE constexpr object_ref_storage(std::nullptr_t) noexcept;

            /// \brief Copies storage from an object, handle or pointer
            template<class Handle>
            NTW_INLINE explicit constexpr object_ref_storage(
                const Handle& other) noexcept;

            /// \brief Copies storage from an object, handle or pointer
            template<class Handle>
            NTW_INLINE constexpr object_ref_storage&
            operator=(const Handle& other) noexcept;

            /// \brief Returns the stored value
            NTW_INLINE constexpr void* get() const;
        };

        class unique_object_storage {
            void* _value = nullptr;

        public:
            /// \brief Constructs storage which does not own a handle
            NTW_INLINE constexpr unique_object_storage() noexcept = default;

            /// \brief Constructs storage which does not own a handle
            NTW_INLINE constexpr unique_object_storage(std::nullptr_t) noexcept;

            /// \brief Constructs storage which owns h
            NTW_INLINE constexpr explicit unique_object_storage(void* h) noexcept;

            /// \brief Constructs storage which owns ref
            NTW_INLINE constexpr explicit unique_object_storage(
                const object_ref_storage& ref) noexcept;

            /// \brief Constructs storage by transferring ownership from other to *this
            NTW_INLINE constexpr unique_object_storage(
                unique_object_storage&& other) noexcept;

            /// \brief Transfers ownership from other to *this
            NTW_INLINE constexpr unique_object_storage& operator=(
                unique_object_storage&& other) noexcept;

            /// \brief Releases ownership
            NTW_INLINE constexpr unique_object_storage& operator=(
                std::nullptr_t) noexcept;

            /// \brief Returns the stored value
            NTW_INLINE constexpr void* get() const;

            /// \brief Releases ownership of the stored value
            NTW_INLINE constexpr void* release();
        };

    } // namespace detail

    using unique_object = basic_object<detail::unique_object_storage>;
    using object_ref    = basic_object<detail::object_ref_storage>;

} // namespace ntw::ob

#include "../../impl/obj/object.inl"

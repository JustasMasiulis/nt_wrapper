#pragma once
#include "../detail/unwrap.hpp"
#include "attributes.hpp"
#include "../status.hpp"
#include "../access.hpp"
#include "object.hpp"

namespace ntw::ob {

    /// \brief Extends access_builder to contain all process specific access flags.
    struct process_access : access_builder<process_access> {
        /// \brief Enables PROCESS_TERMINATE flag
        NTW_INLINE constexpr process_access& terminate();

        /// \brief Enables PROCESS_CREATE_THREAD flag
        NTW_INLINE constexpr process_access& create_thread();

        /// \brief Enables PROCESS_SET_SESSIONID flag
        NTW_INLINE constexpr process_access& set_session_id();

        /// \brief Enables PROCESS_VM_OPERATION flag
        NTW_INLINE constexpr process_access& vm_operation();

        /// \brief Enables PROCESS_VM_READ flag
        NTW_INLINE constexpr process_access& vm_read();

        /// \brief Enables PROCESS_VM_WRITE flag
        NTW_INLINE constexpr process_access& vm_write();

        /// \brief Enables PROCESS_DUP_HANDLE flag
        NTW_INLINE constexpr process_access& dup_handle();

        /// \brief Enables PROCESS_CREATE_PROCESS flag
        NTW_INLINE constexpr process_access& create_process();

        /// \brief Enables PROCESS_SET_QUOTA flag
        NTW_INLINE constexpr process_access& set_qouta();

        /// \brief Enables PROCESS_SET_INFORMATION flag
        NTW_INLINE constexpr process_access& set_info();

        /// \brief Enables PROCESS_QUERY_INFORMATION flag
        NTW_INLINE constexpr process_access& query_info();

        /// \brief Enables PROCESS_SUSPEND_RESUME flag
        NTW_INLINE constexpr process_access& suspend_resume();

        /// \brief Enables PROCESS_QUERY_LIMITED_INFORMATION flag
        NTW_INLINE constexpr process_access& query_limited_info();

        /// \brief Enables PROCESS_SET_LIMITED_INFORMATION flag
        NTW_INLINE constexpr process_access& set_limited_info();

        /// \brief Enables PROCESS_ALL_ACCESS flag
        NTW_INLINE constexpr process_access& all();
    };

    /// \brief Wrapper class around process object and its information
    template<class Handle>
    struct basic_process : Handle {
        /// \brief The type of handle that is used internally
        using handle_type = Handle;

        /// \brief Inherits constructors from handle type.
        using handle_type::handle_type;
        using handle_type::operator=;

        /// \brief Constructs process with the current processes handle.
        NTW_INLINE basic_process();

        /// \brief Opens process using given process id, access and attributes
        /// \param pid Process ID of any type convertible to void*
        /// \param access The access to request for when opening process.
        /// \param attr Optional extra attributes.
        template<class ProcessIdType>
        NTW_INLINE static result<basic_process>
        open(ProcessIdType     pid,
                               process_access    access,
                               const attributes& attr = {}) noexcept;

        /// \brief Reads memory at given address into given range.
        /// \param addr Address of memory to read from.
        /// \param range The range of memory to read into.
        template<class Address, class Range>
        NTW_INLINE status read_mem(Address addr, Range&& range) noexcept;

        /// \brief Reads memory at given address into given buffer.
        /// \param addr Address of memory to read from.
        /// \param buffer Buffer to read into.
        /// \param size Size of buffer.
        template<class Address>
        NTW_INLINE status read_mem(Address addr, void* buffer, std::size_t size) noexcept;

        /// \brief Writes memory to given address from given range.
        /// \param addr Address of memory to write to.
        /// \param range The range of memory to write.
        template<class Address, class Range>
        NTW_INLINE status write_mem(Address addr, Range&& range) noexcept;

        /// \brief Writes memory to given address from given range.
        /// \param addr Address of memory to write to.
        /// \param buffer Buffer to write into addr.
        /// \param size Size of buffer.
        template<class Address>
        NTW_INLINE status write_mem(Address addr, const void* buffer, std::size_t size) noexcept;

        template<class InfoType, class Address>
        NTW_INLINE result<InfoType> query_mem(Address address) noexcept;
    };

    using unique_process = basic_process<unique_object>;
    using process_ref    = basic_process<object_ref>;

} // namespace ntw::ob

#include "../../../impl/ob/process.inl"

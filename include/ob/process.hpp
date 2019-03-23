#pragma once
#include "../detail/unwrap.hpp"
#include "attributes.hpp"
#include "../status.hpp"
#include "../access.hpp"

namespace ntw::ob {

    /// \brief Extends access_builder to contain all process specific access
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
        Handle _handle;

    public:
        /// \brief The type of handle that is used internally
        using handle_type = Handle;

        /// \brief Constructs process with the current processes handle.
        NTW_INLINE basic_process();

        /// \brief Constructs process with given handle
        template<class H>
        NTW_INLINE basic_process(const H& handle);

        /// \brief Returns the internal handle
        NTW_INLINE handle_type& handle();

        /// \brief Returns the internal handle
        NTW_INLINE const handle_type& handle() const;

        /// \brief Opens process using given process id, access and attributes
        /// \param pid Process ID of any type convertible to void*
        /// \param access The access to request for when opening process.
        /// \param attr Optional extra attributes.
        template<class ProcessIdType>
        NTW_INLINE status open(ProcessIdType     pid,
                               process_access    access,
                               const attributes& attr = {});
    };

} // namespace ntw::ob

#include "../../impl/obj/process.inl"

/*
 * Copyright 2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "attributes.hpp"
#include "object.hpp"
#include "../access.hpp"

namespace ntw::ob {

    /// \brief Extends access_builder to contain all job specific access flags.
    struct job_access : access_builder<job_access> {
        /// \brief Enables JOB_OBJECT_ASSIGN_PROCESS flag
        NTW_INLINE constexpr job_access& assign_process();

        /// \brief Enables JOB_OBJECT_SET_ATTRIBUTES flag
        NTW_INLINE constexpr job_access& set_attributes();

        /// \brief Enables JOB_OBJECT_QUERY flag
        NTW_INLINE constexpr job_access& query();

        /// \brief Enables JOB_OBJECT_TERMINATE flag
        NTW_INLINE constexpr job_access& terminate();

        /// \brief Enables JOB_OBJECT_SET_SECURITY_ATTRIBUTES flag
        NTW_INLINE constexpr job_access& set_security_attr();

        /// \brief Enables JOB_OBJECT_IMPERSONATE flag
        NTW_INLINE constexpr job_access& impersonate();

        /// \brief Enables JOB_OBJECT_ALL_ACCESS flag
        NTW_INLINE constexpr job_access& all();
    };

    /// \brief Wrapper class around process object and its information
    template<class Handle>
    struct basic_job : Handle {
        /// \brief The type of handle that is used internally
        using handle_type = Handle;
        using access_type = job_access;

        /// \brief Inherits constructors from handle type.
        using handle_type::handle_type;
        using handle_type::operator=;

        /// \brief Constructs process with the current processes handle.
        NTW_INLINE basic_job() = default;

        /// \brief Opens job using given name, access and attributes.
        /// \param name The name of job object.
        /// \param access The access to request for when opening job.
        /// \param attr Optional extra attributes.
        NTW_INLINE static result<basic_job> open(unicode_string    name,
                                                 job_access        access,
                                                 const attributes& attr = {}) noexcept;

        /// \brief Creates job object using given access.
        /// \param access The access to request for when creating job.
        NTW_INLINE static result<basic_job> create(
            job_access access = job_access{}.all()) noexcept;

        /// \brief Creates job using given name, access and attributes.
        /// \param name The name of job object.
        /// \param access The access to request for when opening job.
        /// \param attr Optional extra attributes.
        NTW_INLINE static result<basic_job> create(unicode_string name,
                                                   job_access access = job_access{}.all(),
                                                   const attributes& attr = {}) noexcept;

        /// \brief Assigns a process to the job object.
        /// \param handle The process handle.
        template<class ProcessHandle>
        NTW_INLINE status assign_process(const ProcessHandle& handle) const noexcept;

        /// \brief Assigns current process to the job object.
        NTW_INLINE status assign_curr_process() const noexcept;

        /// \brief Checks if given process is assigned this job
        /// \param process The process to check
        template<class ProcessHandle>
        NTW_INLINE result<bool> is_assigned(const ProcessHandle& process) const noexcept;

        /// \brief Terminates all processes associated with job.
        /// \param status The exit status for processes.
        NTW_INLINE ntw::status terminate(ntw::status status) const noexcept;

        /// \brief Sets job information
        /// \param info The information to set
        template<class T>
        NTW_INLINE ntw::status set(const T& info) const noexcept;

        /// \brief Queries job information.
        /// \tparam T The information type
        template<class T>
        NTW_INLINE ntw::result<T> query() const noexcept;
    };

    using job     = basic_job<object>;
    using job_ref = basic_job<object_ref>;

} // namespace ntw::ob

#include "impl/job.inl"
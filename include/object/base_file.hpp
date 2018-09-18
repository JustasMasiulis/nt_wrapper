/*
 * Copyright 2018 Justas Masiulis
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
#include "handle.hpp"
#include "../util.hpp"

namespace ntw::obj {

    class file_options_builder {
        ACCESS_MASK _access = 0;

    public:
        // ShareAccess; multiple allowed
        NTW_INLINE constexpr file_options_builder& reset_share();

        NTW_INLINE constexpr file_options_builder& share_all(); 
        NTW_INLINE constexpr file_options_builder& share_read(); // FILE_SHARE_READ
        NTW_INLINE constexpr file_options_builder& share_write(); // FILE_SHARE_WRITE
        NTW_INLINE constexpr file_options_builder& share_delete(); // FILE_SHARE_DELETE

        // CreateDisposition; 1 allowed
        NTW_INLINE constexpr file_options_builder& supersede(); // FILE_SUPERSEDE
        NTW_INLINE constexpr file_options_builder& create(); // FILE_CREATE
        NTW_INLINE constexpr file_options_builder& open(); // FILE_OPEN
        NTW_INLINE constexpr file_options_builder& open_or_create(); // FILE_OPEN_IF
        NTW_INLINE constexpr file_options_builder& overwrite(); // FILE_OVERWRITE
        NTW_INLINE constexpr file_options_builder& overwrite_or_create(); // FILE_OVERWRITE_IF

        // CreateOptions; multiple allowed
        NTW_INLINE constexpr file_options_builder& reset_create_options();

        NTW_INLINE constexpr file_options_builder& directory(); // FILE_DIRECTORY_FILE
        NTW_INLINE constexpr file_options_builder& non_directory(); // FILE_NON_DIRECTORY_FILE
        NTW_INLINE constexpr file_options_builder& write_trough(); // FILE_WRITE_THROUGH
        NTW_INLINE constexpr file_options_builder& sequential_access(); // FILE_SEQUENTIAL_ONLY
        NTW_INLINE constexpr file_options_builder& random_access(); // FILE_RANDOM_ACCESS
        NTW_INLINE constexpr file_options_builder& sync_io_alert(); // FILE_SYNCHRONOUS_IO_ALERT
        NTW_INLINE constexpr file_options_builder& sync_io_nonalert(); // FILE_SYNCHRONOUS_IO_NONALERT
        NTW_INLINE constexpr file_options_builder& create_tree_connection(); // FILE_CREATE_TREE_CONNECTION
        NTW_INLINE constexpr file_options_builder& no_ea_knownledge(); // FILE_NO_EA_KNOWLEDGE
        NTW_INLINE constexpr file_options_builder& open_reparse_point(); // FILE_OPEN_REPARSE_POINT
        NTW_INLINE constexpr file_options_builder& delete_on_close(); // FILE_DELETE_ON_CLOSE
        NTW_INLINE constexpr file_options_builder& open_by_file_id(); // FILE_OPEN_BY_FILE_ID
        NTW_INLINE constexpr file_options_builder& open_for_backup(); // FILE_OPEN_FOR_BACKUP_INTENT
        NTW_INLINE constexpr file_options_builder& reserve_opfilter(); // FILE_RESERVE_OPFILTER
        NTW_INLINE constexpr file_options_builder& requires_oplock(); // FILE_OPEN_REQUIRING_OPLOCK
        NTW_INLINE constexpr file_options_builder& complete_if_oplocked(); // FILE_COMPLETE_IF_OPLOCKED

        // DesiredAccess; multiple allowed
        NTW_INLINE constexpr file_options_builder& reset_access();

        NTW_INLINE constexpr file_options_builder& deleteable(); // DELETE
        NTW_INLINE constexpr file_options_builder& synchronizable(); // SYNCHRONIZE
        NTW_INLINE constexpr file_options_builder& executeable(); // FILE_EXECUTE
        NTW_INLINE constexpr file_options_builder& traversible(); // FILE_TRAVERSE
        NTW_INLINE constexpr file_options_builder& listable_directory(); // FILE_LIST_DIRECTORY

        NTW_INLINE constexpr file_options_builder& generic_readable(); // FILE_GENERIC_READ
        NTW_INLINE constexpr file_options_builder& generic_writeable(); // FILE_GENERIC_WRITE
        NTW_INLINE constexpr file_options_builder& generic_executeable(); // FILE_GENERIC_EXECUTE

        NTW_INLINE constexpr file_options_builder& readable_data(); // FILE_READ_DATA
        NTW_INLINE constexpr file_options_builder& readable_attributes(); // FILE_READ_ATTRIBUTES
        NTW_INLINE constexpr file_options_builder& readable_extended_attributes(); // FILE_READ_EA
        NTW_INLINE constexpr file_options_builder& readable_access_control(); // READ_CONTROL

        NTW_INLINE constexpr file_options_builder& writeable_data(); // FILE_WRITE_DATA
        NTW_INLINE constexpr file_options_builder& writeable_attributes(); // FILE_WRITE_ATTRIBUTES
        NTW_INLINE constexpr file_options_builder& writeable_extended_attributes(); // FILE_WRITE_EA
        NTW_INLINE constexpr file_options_builder& writeable_access_control(); // WRITE_DAC
        NTW_INLINE constexpr file_options_builder& writeable_ownership(); // WRITE_OWNER
        NTW_INLINE constexpr file_options_builder& appendable_data(); // FILE_APPEND_DATA
    };

    namespace detail {

        template<class Handle>
        class basic_base_file {
            Handle _handle;

            NT_FN _open(UNICODE_STRING path,
                        bool           create_if_not_existing,
                        unsigned long  access_flags,
                        unsigned long  create_flags) noexcept;

        public:
            NTW_INLINE basic_base_file()  = default;
            NTW_INLINE ~basic_base_file() = default;

            template<class ObjectHandle>
            NTW_INLINE basic_base_file(const ObjectHandle& handle)
                : _handle(unwrap_handle(handle))
            {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }

            constexpr static unsigned long default_access =
                FILE_READ_DATA | FILE_WRITE_DATA | DELETE;

            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN open_dir(const StringRef& path,
                           bool             create_if_not_existing = false,
                           unsigned long    access_flags = default_access) noexcept;

            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN open(const StringRef& path,
                       unsigned long    access_flags = default_access) noexcept;

            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN create(const StringRef& path,
                         bool             open_if_exists = false,
                         unsigned long    access_flags   = default_access) noexcept;

            NT_FN open_as_pipe(UNICODE_STRING name,
                               unsigned long  in_max,
                               unsigned long  out_max,
                               unsigned long  instances = -1,
                               LARGE_INTEGER  timeout = make_large_int(-500000)) noexcept;

            NT_FN size(std::uint64_t& size_out) const noexcept;

            template<class Callback, class... Args>
            NT_FN enum_contained_files(Callback callback, Args&&... args) const noexcept;

            template<class InBuffer, class OutBuffer>
            NT_FN device_io_control(unsigned long   control_code,
                                    const InBuffer& in_buffer,
                                    OutBuffer&      out_buffer,
                                    unsigned long*  bytes_returned = nullptr) const
                noexcept;

            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN static destroy(const StringRef& path,
                                 bool             case_sensitive = false) noexcept;
        };

    } // namespace detail

} // namespace ntw::obj

#include "../impl/file.inl"

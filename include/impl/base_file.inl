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
#include "../object/base_file.hpp"

#define NTW_FILE_OPTION(function_name, member, value, op)                               \
   NTW_FILE_OPTION( file_options_builder::function_name() \
    {                                                                                \
        member op value;                                                             \
        return *this;                                                                \
    }

namespace ntw::obj {

    NTW_INLINE constexpr unsigned long file_options_builder::share_access() const
    {
        return _share_access;
    }

    NTW_INLINE constexpr unsigned long file_options_builder::disposition() const
    {
        return _disposition;
    }

    NTW_INLINE constexpr unsigned long file_options_builder::attributes() const
    {
        return _attributes;
    }

    NTW_INLINE constexpr unsigned long file_options_builder::create_options() const
    {
        return _options;
    }

    NTW_INLINE constexpr ACCESS_MASK file_options_builder::access() const
    {
        return _access;
    }


    NTW_FILE_OPTION(reset_share_access, _share_access, 0, =)
    NTW_FILE_OPTION(share_read, _share_access, FILE_SHARE_READ, |=)
    NTW_FILE_OPTION(share_write, _share_access, FILE_SHARE_WRITE, |=)
    NTW_FILE_OPTION(share_delete, _share_access, FILE_SHARE_DELETE, |=)

    NTW_FILE_OPTION(reset_disposition, _disposition, 0, =)
    NTW_FILE_OPTION(open, _disposition, FILE_OPEN, =)
    NTW_FILE_OPTION(create, _disposition, FILE_CREATE, =)
    NTW_FILE_OPTION(supersede, _disposition, FILE_SUPERSEDE, =)
    NTW_FILE_OPTION(overwrite, _disposition, FILE_OVERWRITE, =)
    NTW_FILE_OPTION(open_or_create, _disposition, FILE_OPEN_IF, =)
    NTW_FILE_OPTION(overwrite_or_create, _disposition, FILE_OVERWRITE_IF, =)

    NTW_FILE_OPTION(directory, _options, FILE_DIRECTORY_FILE, |=)
    NTW_FILE_OPTION(non_directory, _options, FILE_NON_DIRECTORY_FILE, |=)
    NTW_FILE_OPTION(write_trough, _options, FILE_WRITE_THROUGH, |=)
    NTW_FILE_OPTION(sequential_access, _options, FILE_SEQUENTIAL_ONLY, |=)
    NTW_FILE_OPTION(random_access, _options, FILE_RANDOM_ACCESS, |=)
    NTW_FILE_OPTION(sync_io_alert, _options, FILE_SYNCHRONOUS_IO_ALERT, |=)
    NTW_FILE_OPTION(sync_io_nonalert, _options, FILE_SYNCHRONOUS_IO_NONALERT, |=)
    NTW_FILE_OPTION(create_tree_connection, _options, FILE_CREATE_TREE_CONNECTION, |=)
    NTW_FILE_OPTION(no_ea_knownledge, _options, FILE_NO_EA_KNOWLEDGE, |=)
    NTW_FILE_OPTION(open_reparse_point, _options, FILE_OPEN_REPARSE_POINT, |=)
    NTW_FILE_OPTION(delete_on_close, _options, FILE_DELETE_ON_CLOSE, |=)
    NTW_FILE_OPTION(open_by_file_id, _options, FILE_OPEN_BY_FILE_ID, |=)
    NTW_FILE_OPTION(open_for_backup, _options, FILE_OPEN_FOR_BACKUP_INTENT, |=)
    NTW_FILE_OPTION(reserve_opfilter, _options, FILE_RESERVE_OPFILTER, |=)
    NTW_FILE_OPTION(requires_oplock, _options, FILE_OPEN_REQUIRING_OPLOCK, |=)
    NTW_FILE_OPTION(complete_if_oplocked, _options, FILE_COMPLETE_IF_OPLOCKED, |=)

    NTW_FILE_OPTION(reset_access, _access, 0, =);

    NTW_FILE_OPTION(full_access, _access, GENERIC_ALL, =);

    NTW_FILE_OPTION(deleteable, _access, DELETE, |=)
    NTW_FILE_OPTION(synchronizable, _access, SYNCHRONIZE, |=)
    NTW_FILE_OPTION(executeable, _access, FILE_EXECUTE, |=)
    NTW_FILE_OPTION(traversible, _access, FILE_TRAVERSE, |=)
    NTW_FILE_OPTION(listable_directory, _access, FILE_LIST_DIRECTORY, |=)

    NTW_FILE_OPTION(generic_readable, _access, FILE_GENERIC_READ, |=)
    NTW_FILE_OPTION(generic_writeable, _access, FILE_GENERIC_WRITE, |=)
    NTW_FILE_OPTION(generic_executeable, _access, FILE_GENERIC_EXECUTE, |=)

    NTW_FILE_OPTION(readable_data, _access, FILE_READ_DATA, |=)
    NTW_FILE_OPTION(readable_attributes, _access, FILE_READ_ATTRIBUTES, |=)
    NTW_FILE_OPTION(readable_extended_attributes, _access, FILE_READ_EA, |=)
    NTW_FILE_OPTION(readable_access_control, READ_CONTROL, |=)

    NTW_FILE_OPTION(writeable_data, _access, FILE_WRITE_DATA, |=)
    NTW_FILE_OPTION(writeable_attributes, _access, FILE_WRITE_ATTRIBUTES, |=)
    NTW_FILE_OPTION(writeable_extended_attributes, _access, FILE_WRITE_EA, |=)
    NTW_FILE_OPTION(writeable_access_control, _access, WRITE_DAC, |=)
    NTW_FILE_OPTION(writeable_ownership, _access, WRITE_OWNER, |=)
    NTW_FILE_OPTION(appendable_data, _access, FILE_APPEND_DATA, |=)

    // FileAttributes; multiple allowed
    NTW_FILE_OPTION(reset_attributes, _attributes, 0, =);

    NTW_FILE_OPTION(archive, _attributes, FILE_ATTRIBUTE_ARCHIVE, |=)
    NTW_FILE_OPTION(encrypted, _attributes, FILE_ATTRIBUTE_ENCRYPTED, |=)
    NTW_FILE_OPTION(hidden, _attributes, FILE_ATTRIBUTE_HIDDEN, |=)
    NTW_FILE_OPTION(normal, _attributes, FILE_ATTRIBUTE_NORMAL, |=)
    NTW_FILE_OPTION(offline, _attributes, FILE_ATTRIBUTE_OFFLINE, |=)
    NTW_FILE_OPTION(readonly, _attributes, FILE_ATTRIBUTE_READONLY, |=)
    NTW_FILE_OPTION(system, _attributes, FILE_ATTRIBUTE_SYSTEM, |=)
    NTW_FILE_OPTION(temporary, _attributes, FILE_ATTRIBUTE_TEMPORARY, |=)

} // namespace ntw::obj

#undef NTW_FILE_OPTION

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
#include "../ob/object.hpp"
#include "../result.hpp"

namespace ntw::io {

    namespace detail {

        class file_attributes_builder {
            struct file_attributes_data {
                ulong_t attributes               = 0;
                constexpr file_attributes_data() = default;
            };

            file_attributes_data _data;

        protected:
            constexpr file_attributes_builder() = default;
            ~file_attributes_builder()          = default;

        public:
            NTW_INLINE constexpr ulong_t attributes() const;

            // clang-format off
			// FileAttributes; multiple allowed
			NTW_INLINE constexpr file_attributes_builder& reset_attributes();

			// TOOD: add bool args to be able to toggle flags more easily
			NTW_INLINE constexpr file_attributes_builder& archive(); // FILE_ATTRIBUTE_ARCHIVE
			NTW_INLINE constexpr file_attributes_builder& encrypted(); // FILE_ATTRIBUTE_ENCRYPTED
			NTW_INLINE constexpr file_attributes_builder& hidden(); // FILE_ATTRIBUTE_HIDDEN
			NTW_INLINE constexpr file_attributes_builder& normal(); // FILE_ATTRIBUTE_NORMAL
			NTW_INLINE constexpr file_attributes_builder& offline(); // FILE_ATTRIBUTE_OFFLINE
			NTW_INLINE constexpr file_attributes_builder& readonly(); // FILE_ATTRIBUTE_READONLY
			NTW_INLINE constexpr file_attributes_builder& system(); // FILE_ATTRIBUTE_SYSTEM
			NTW_INLINE constexpr file_attributes_builder& temporary(); // FILE_ATTRIBUTE_TEMPORARY
            // clang-format on
        };

        struct pipe_options_data {
            ulong_t      inbound_qouta   = 0;
            ulong_t      outbound_qouta  = 0;
            ulong_t      type            = 0;
            ulong_t      instances_limit = -1;
            std::int64_t timeout         = -500000;
        };

        class pipe_options_builder {
            pipe_options_data _data;

        protected:
            constexpr pipe_options_builder() = default;
            ~pipe_options_builder()          = default;

        public:
            NTW_INLINE constexpr const pipe_options_data& pipe_data() const;

            //  clang-format off
            // must be specified
            NTW_INLINE constexpr pipe_options_builder& qouta(ulong_t inbound,
                                                             ulong_t outbound);
            NTW_INLINE constexpr pipe_options_builder& inbound_qouta(ulong_t qouta);
            NTW_INLINE constexpr pipe_options_builder& outbound_qouta(ulong_t qouta);

            // NamedPipeType; byte stream is default; 1 allowed
            // resets NamedPipeType back to byte stream and clears other flags
            NTW_INLINE constexpr pipe_options_builder& reset_type();

            // also sets ReadMode to the same values
            NTW_INLINE constexpr pipe_options_builder&
            byte_stream(); // FILE_PIPE_BYTE_STREAM_TYPE
            NTW_INLINE constexpr pipe_options_builder&
            message_stream(); // FILE_PIPE_MESSAGE_TYPE

            // NamedPipeType; may be specified
            NTW_INLINE constexpr pipe_options_builder&
            accept_remote_clients(); // FILE_PIPE_ACCEPT_REMOTE_CLIENTS
            NTW_INLINE constexpr pipe_options_builder&
            reject_remote_clients(); // FILE_PIPE_REJECT_REMOTE_CLIENTS

            // default = unlimited
            NTW_INLINE constexpr pipe_options_builder& instances_limit(ulong_t limit);

            // default = 5 seconds
            NTW_INLINE constexpr pipe_options_builder& timeout(std::int64_t nanoseconds);
            // clang-format on
        };

        struct file_options_data {
            ACCESS_MASK access       = 0;
            ulong_t     share_access = 0;
            ulong_t     options      = 0;
        };

        template<class Base>
        class file_options_builder : public Base {
            file_options_data _data;
            //  NOTE: if a need arises for extended attributes support please open a
            //  ticket
            // and I'll add a function and data members for it

        public:
            constexpr file_options_builder() = default;

            NTW_INLINE constexpr const file_options_data& data() const;

            NTW_INLINE constexpr file_options_builder copy() const;

            // clang-format off
			// ShareAccess; multiple allowed
			NTW_INLINE constexpr file_options_builder& reset_share_access();

			NTW_INLINE constexpr file_options_builder& share_all(); 
			NTW_INLINE constexpr file_options_builder& share_read(); // FILE_SHARE_READ
			NTW_INLINE constexpr file_options_builder& share_write(); // FILE_SHARE_WRITE
			NTW_INLINE constexpr file_options_builder& share_delete(); // FILE_SHARE_DELETE

			// CreateOptions; multiple allowed
			NTW_INLINE constexpr file_options_builder& reset_create_options();

			NTW_INLINE constexpr file_options_builder& write_trough(); // FILE_WRITE_THROUGH
			NTW_INLINE constexpr file_options_builder& sequential_access(); // FILE_SEQUENTIAL_ONLY
			NTW_INLINE constexpr file_options_builder& random_access(); // FILE_RANDOM_ACCESS
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

			NTW_INLINE constexpr file_options_builder& full_access();

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
            // clang-format on
        };

    } // namespace detail

    using file_options = detail::file_options_builder<detail::file_attributes_builder>;
    using pipe_options = detail::file_options_builder<detail::pipe_options_builder>;

    namespace detail {

        /// \brief Contains APIs that are common between basic_file and async_file
        template<class Derived, class Traits>
        class base_file {
            NTW_INLINE static result<Derived> _open(const unicode_string& path,
                                                    const ob::attributes& attributes,
                                                    const file_options&   opt,
                                                    ulong_t disposition) noexcept;

        protected:
            NTW_INLINE ~base_file() = default;

        public:
            constexpr static auto options = Traits::options;

            /// \brief Opens file using NtCreateFile API. FILE_OPEN disposition is used.
            /// \param path The path to file.
            ///             May be either an UNICODE_STRING or std::wstring_view.
            /// \param options The options used while opening the file.
            NTW_INLINE static result<Derived> open(
                const unicode_string& path,
                const ob::attributes& attributes = {},
                const file_options&   opt        = options) noexcept;

            /// \brief Opens file using NtCreateFile API. FILE_CREATE disposition is used.
            /// \param path The path to file.
            ///             May be either an UNICODE_STRING or std::wstring_view.
            /// \param options The options used while opening the file.
            NTW_INLINE static result<Derived> create(
                const unicode_string& path,
                const ob::attributes& attributes = {},
                const file_options&   opt        = options) noexcept;

            /// \brief Opens file using NtCreateFile API. FILE_SUPERSEDE disposition is
            ///        used.
            /// \param path The path to file.
            ///             May be either an UNICODE_STRING or std::wstring_view.
            /// \param options The options used while opening the file.
            NTW_INLINE static result<Derived> supersede(
                const unicode_string& path,
                const ob::attributes& attributes = {},
                const file_options&   opt        = options) noexcept;

            /// \brief Opens file using NtCreateFile API. FILE_OVERWRITE disposition is
            ///        used.
            /// \param path The path to file.
            ///             May be either an UNICODE_STRING or std::wstring_view.
            /// \param options The options used while opening the file.
            NTW_INLINE static result<Derived> overwrite(
                const unicode_string& path,
                const ob::attributes& attributes = {},
                const file_options&   opt        = options) noexcept;

            /// \brief Opens file using NtCreateFile API. FILE_OPEN_IF disposition is
            ///        used.
            /// \param path The path to file.
            ///             May be either an UNICODE_STRING or std::wstring_view.
            /// \param options The options used while opening the file.
            NTW_INLINE static result<Derived> open_or_create(
                const unicode_string& path,
                const ob::attributes& attributes = {},
                const file_options&   opt        = options) noexcept;

            /// \brief Opens file using NtCreateFile API. FILE_OVERWRITE_IF disposition is
            ///        used.
            /// \param path The path to file.
            ///             May be either an UNICODE_STRING or std::wstring_view.
            /// \param options The options used while opening the file.
            NTW_INLINE static result<Derived> overwrite_or_create(
                const unicode_string& path,
                const ob::attributes& attributes = {},
                const file_options&   opt        = options) noexcept;

            /// \brief Queries opened file size using NtQueryInformationFile API.
            NTW_INLINE result<std::uint64_t> size() const noexcept;

            /// \brief Flushes the file buffer.
            NTW_INLINE status flush() const noexcept;

            /// \brief Deletes opened file using NtDeleteFile API.
            /// \param path The path to file.
            NTW_INLINE status static destroy(
                unicode_string path, const ob::attributes& attributes = {}) noexcept;
        };

        NTW_INLINE constexpr ulong_t normalize_attributes(
            const file_options& options) noexcept;

        template<bool Sync, class Options>
        NTW_INLINE constexpr ulong_t synchronize_options(const Options& options) noexcept;

        template<bool Sync, class Options>
        NTW_INLINE constexpr ulong_t synchronize_access(const Options& options) noexcept;

    } // namespace detail

} // namespace ntw::io

#include "impl/base_file.inl"
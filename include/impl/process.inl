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
#include "../object/process.hpp"
#include "../detail/generic_query.hpp"
#include "../detail/subsystem.hpp"
#include "../native_peb.hpp"

namespace ntw::obj {
    namespace detail {

        template<class P>
        struct peb_t {
            std::uint8_t _ignored[4];
            P            _ignored2[2];
            P            Ldr;
        };

        template<class P>
        struct list_entry_t {
            P Flink;
            P Blink;
        };

        template<class P>
        struct peb_ldr_data_t {
            unsigned long   Length;
            bool            Initialized;
            P               SsHandle;
            list_entry_t<P> InLoadOrderModuleList;
        };

        template<class P>
        struct unicode_string_t {
            std::uint16_t Length;
            std::uint16_t MaximumLength;
            P             Buffer;
        };

        template<class P>
        struct ldr_data_table_entry_t {
            list_entry_t<P> InLoadOrderLinks;
            list_entry_t<P> InMemoryOrderLinks;
            union {
                list_entry_t<P> InInitializationOrderLinks;
                list_entry_t<P> InProgressLinks;
            };
            P                   DllBase;
            P                   EntryPoint;
            unsigned long       SizeOfImage;
            unicode_string_t<P> FullDllName;
        };

        NTW_INLINE bool is_process_same_arch(
            const PROCESS_EXTENDED_BASIC_INFORMATION& info) noexcept
        {
            return static_cast<bool>(info.IsWow64Process) ==
                   static_cast<bool>(local_teb().WowTebOffset);
        }

        struct enum_modules_t {
            template<class Subsystem, class Callback>
            NT_FN operator()(void* handle, Callback cb, std::uint64_t peb_address) const
                noexcept
            {
                using ptr_t = typename Subsystem::target_pointer;

                ptr_t Ldr;
                ret_on_err(Subsystem::read(handle,
                                           static_cast<ptr_t>(peb_address) +
                                               offsetof(peb_t<ptr_t>, Ldr),
                                           Ldr));

                const auto list_head =
                    Ldr + offsetof(peb_ldr_data_t<ptr_t>, InLoadOrderModuleList);

                ptr_t load_order_modules_list_flink;
                ret_on_err(
                    Subsystem::read(handle, list_head, load_order_modules_list_flink));

                ldr_data_table_entry_t<ptr_t> entry;
                process_module_info           info;

                for(auto list_curr = load_order_modules_list_flink;
                    list_curr != list_head;) {
                    ret_on_err(Subsystem::read(handle, list_curr, entry));
                    list_curr = entry.InLoadOrderLinks.Flink;

                    // don't think that this can happen, but just to be on the safe side
                    if(entry.FullDllName.Length > MAX_PATH * sizeof(wchar_t)) {
                        // NTW_DEBUG_TRACE("entry.FullDllName.Length > MAX_PATH * 2");
                        continue;
                    }

                    ret_on_err(Subsystem::read(handle,
                                               entry.FullDllName.Buffer,
                                               info._full_path,
                                               entry.FullDllName.Length));

                    info._address  = entry.DllBase;
                    info._size     = entry.SizeOfImage;
                    info._path_len = entry.FullDllName.Length / sizeof(wchar_t);

                    NTW_CALLBACK_BREAK_IF_FALSE(cb, info);
                }

                return STATUS_SUCCESS;
            }
        };

        template<class H>
        template<class Callback>
        NT_FN basic_process<H>::enum_modules(Callback cb) const noexcept
        {
            PROCESS_EXTENDED_BASIC_INFORMATION ext_info;
            ext_info.Size = sizeof(ext_info);
            ret_on_err(this->info(ProcessBasicInformation, ext_info));
            const bool same_arch = is_process_same_arch(ext_info);

            std::uint64_t peb_addr;
            if(same_arch)
                peb_addr =
                    reinterpret_cast<std::uintptr_t>(ext_info.BasicInfo.PebBaseAddress);
            else
                ret_on_err(peb_address(peb_addr, false));

            return ntw::detail::subsystem_call(
                same_arch, enum_modules_t{}, _handle.get(), std::move(cb), peb_addr);
        }

        template<class H>
        template<class PidT>
        NT_FN basic_process<H>::open(PidT pid, ACCESS_MASK access) noexcept
        {
            auto      attributes = make_attributes(nullptr, 0);
            CLIENT_ID cid{ reinterpret_cast<void*>(pid), nullptr };

            void*      temp_handle = nullptr;
            const auto status =
                LI_NT(NtOpenProcess)(&temp_handle, access, &attributes, &cid);

            if(NT_SUCCESS(status))
                _handle.reset(temp_handle);

            return status;
        }

        template<class H>
        template<class Buffer, class>
        NT_FN basic_process<H>::info(PROCESSINFOCLASS info_class,
                                     Buffer&          buffer,
                                     unsigned long    size,
                                     unsigned long*   returned) const noexcept
        {
            return LI_NT(NtQueryInformationProcess)(
                _handle.get(), info_class, &buffer, size, returned);
        }

        template<class H>
        template<class Callback, class... Args, class>
        NT_FN basic_process<H>::info(PROCESSINFOCLASS info_class,
                                     Callback         cb,
                                     Args&&... args) const noexcept
        {
            NTW_IMPLEMENT_QUERY_CALLBACK
        }

        template<class H>
        template<class Buffer>
        NT_FN basic_process<H>::set_info(PROCESSINFOCLASS info_class,
                                         Buffer&          buffer,
                                         unsigned long    info_size) const noexcept
        {
            return LI_NT(NtSetInformationProcess)(
                _handle.get(), info_class, &buffer, info_size);
        }

        template<class H>
        template<class IdT>
        NT_FN basic_process<H>::id(IdT& id)
        {
            PROCESS_BASIC_INFORMATION basic_info;
            const auto                status = info(ProcessBasicInformation, basic_info);
            if(NT_SUCCESS(status))
                id = reinterpret_cast<IdT>(basic_info.UniqueProcessId);
            return status;
        }

        template<class H>
        NT_FN basic_process<H>::terminate(NTSTATUS exit_status) const noexcept
        {
            return LI_NT(NtTerminateProcess)(_handle.get(), exit_status);
        }

        template<class H>
        NT_FN basic_process<H>::suspend() const noexcept
        {
            return LI_NT(NtSuspendProcess)(_handle.get());
        }

        template<class H>
        NT_FN basic_process<H>::resume() const noexcept
        {
            return LI_NT(NtResumeProcess)(_handle.get());
        }

        template<class H>
        NT_FN basic_process<H>::peb_address(std::uint64_t& address, bool same_arch) const
            noexcept
        {
            NTSTATUS status;
            if(same_arch) {
                PROCESS_BASIC_INFORMATION pbi;
                status = this->info(ProcessBasicInformation, pbi);

                if(NT_SUCCESS(status))
                    address = reinterpret_cast<std::uintptr_t>(pbi.PebBaseAddress);
            }
            else {
#ifdef _WIN64

                void* peb_ptr;
                status = info(ProcessWow64Information, peb_ptr);
                if(NT_SUCCESS(status))
                    address = reinterpret_cast<std::uintptr_t>(peb_ptr);

#else

                using NtWow64QueryInformationProcess64 =
                    decltype(&NtQueryInformationProcess);

                struct PROCESS_BASIC_INFORMATION64 {
                    NTSTATUS      ExitStatus;
                    std::uint64_t PebBaseAddress;
                    std::uint64_t AffinityMask;
                    KPRIORITY     BasePriority;
                    std::uint64_t UniqueProcessId;
                    std::uint64_t InheritedFromUniqueProcessId;
                } pbi;

                status = LI_NT_DEF(NtWow64QueryInformationProcess64)(
                    _handle.get(), ProcessBasicInformation, &pbi, sizeof(pbi), nullptr);

                if(NT_SUCCESS(status))
                    address = pbi.PebBaseAddress;

#endif
            }

            return status;
        }

        template<class H>
        template<class Cb>
        NT_FN basic_process<H>::enum_fixed_drives(Cb cb) const noexcept
        {
            PROCESS_DEVICEMAP_INFORMATION devicemap_info;
            const auto                    status =
                ntw::obj::process_ref{}.info(ProcessDeviceMap, devicemap_info.Query);
            if(NT_SUCCESS(status))
                for(std::uint8_t i = 0; i < 32; ++i)
                    if(devicemap_info.Query.DriveType[i] == DRIVE_FIXED)
                        cb(i);
            return status;
        }

    } // namespace detail

    NTW_INLINE std::uint64_t process_module_info::base_address() const noexcept
    {
        return _address;
    }

    NTW_INLINE std::uint32_t process_module_info::size() const noexcept { return _size; }

    NTW_INLINE std::wstring_view process_module_info::full_path() const noexcept
    {
        return std::wstring_view(_full_path, _path_len);
    }

    NTW_INLINE std::wstring_view process_module_info::name() const noexcept
    {
        using rit = std::reverse_iterator<const wchar_t*>;
        const rit first(_full_path + _path_len);

        const auto it   = std::find(first, rit(_full_path), L'\\');
        const auto size = (_full_path + _path_len) - it.base();
        return { it.base(), static_cast<typename std::wstring_view::size_type>(size) };
    }

    NTW_INLINE const wchar_t* process_module_info::path_data() const noexcept
    {
        return _full_path;
    }

    NTW_INLINE wchar_t* process_module_info::path_data() noexcept { return _full_path; }

    NTW_INLINE std::uint16_t process_module_info::path_length() const noexcept
    {
        return _path_len;
    }

} // namespace ntw::obj
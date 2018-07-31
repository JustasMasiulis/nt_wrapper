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
#include "../memory.hpp"
#include "../detail/generic_query.hpp"
#include "../util.hpp"
#include <memory>

namespace ntw {

    template<class T>
    template<class Address>
    NT_FN detail::unique_memory_alloc_traits<T>::allocate(Address       addr,
                                                          SIZE_T        sz,
                                                          unsigned long protection)
    {
        const auto status = memory().allocate(addr, sz, protection);
        if(NT_SUCCESS(status)) {
            auto me = static_cast<unique_object<unique_memory_alloc_traits<T>>*>(this);
            me->reset(address_cast<T*>(addr));
        }
        return status;
    }

    template<class T>
    NT_FN detail::unique_memory_alloc_traits<T>::allocate(SIZE_T        sz,
                                                          unsigned long protection)
    {
        return allocate(nullptr, sz, protection);
    }

    template<class T>
    NTW_INLINE void detail::unique_memory_alloc_traits<T>::close(value_type* ptr) const
        noexcept
    {
        if(ptr)
            static_cast<void>(memory().free(ptr));
    }

    template<class Address>
    NT_FN detail::unique_memory_lock_traits::lock(Address address, SIZE_T size) noexcept
    {
        const auto status = memory().lock(address, size);
        if(NT_SUCCESS(status)) {
            auto thisptr = static_cast<unique_object<unique_memory_lock_traits>*>(this);
            thisptr->reset(reinterpret_cast<void*>(address));
            _size = size;
        }

        return status;
    }

    template<class Address>
    NT_FN detail::unique_memory_lock_traits::lock_strong(Address address,
                                                         SIZE_T  size) noexcept
    {
        QUOTA_LIMITS qouta;
        while(true) {
            const auto status = lock(address, size);

            // increase the working set
            if(status == STATUS_WORKING_SET_QUOTA) {
                ret_on_err(LI_NT(NtQueryInformationProcess)(NtCurrentProcess(),
                                                            ProcessQuotaLimits,
                                                            &qouta,
                                                            sizeof(qouta),
                                                            nullptr));
                qouta.MaximumWorkingSetSize += size + 0x1000;

                ret_on_err(
                    LI_NT(NtSetInformationProcess)(NtCurrentProcess(),
                                                   ProcessQuotaLimits,
                                                   &qouta,
                                                   static_cast<unsigned>(sizeof(qouta))));
            }
            else
                return status;
        }
    }

    NTW_INLINE void detail::unique_memory_lock_traits::close(void* ptr) const noexcept
    {
        if(ptr)
            static_cast<void>(memory().unlock(ptr, _size));
    }

    template<class Address>
    NTW_INLINE Address memory::region_info::base() const noexcept
    {
        return address_cast<Address>(_info.BaseAddress);
    }

    NTW_INLINE std::size_t memory::region_info::size() const noexcept
    {
        return _info.RegionSize;
    }

    template<class Address>
    NTW_INLINE Address memory::region_info::end() const noexcept
    {
        return address_cast<Address>(base<std::uintptr_t>() + size());
    }

    NTW_INLINE memory::protection memory::region_info::protection() const noexcept
    {
        return { _info.Protect };
    }

    NTW_INLINE memory::region_info::operator bool() const noexcept
    {
        return _info.State != MEM_FREE;
    }
    NTW_INLINE bool memory::region_info::is_commited() const noexcept
    {
        return _info.State == MEM_COMMIT;
    }
    NTW_INLINE bool memory::region_info::is_reserved() const noexcept
    {
        return _info.State == MEM_RESERVE;
    }
    NTW_INLINE unsigned long memory::region_info::state() const noexcept
    {
        return _info.State;
    }

    NTW_INLINE unsigned long memory::region_info::type() const noexcept
    {
        return _info.Type;
    }
    NTW_INLINE bool memory::region_info::is_mapped() const noexcept
    {
        return _info.Type == MEM_MAPPED;
    }
    NTW_INLINE bool memory::region_info::is_private() const noexcept
    {
        return _info.Type == MEM_PRIVATE;
    }
    NTW_INLINE bool memory::region_info::is_image() const noexcept
    {
        return _info.Type == MEM_IMAGE;
    }

    template<class Address>
    NTW_INLINE Address memory::region_info::allocation_base() const noexcept
    {
        return address_cast<Address>(_info.AllocationBase);
    }

    NTW_INLINE memory::protection memory::region_info::allocation_protection() const
        noexcept
    {
        return { _info.AllocationProtect };
    }

    NTW_INLINE MEMORY_BASIC_INFORMATION& memory::region_info::get() noexcept
    {
        return _info;
    }

    NTW_INLINE constexpr bool memory::protection::accessible() const noexcept
    {
        return !(_native & PAGE_NOACCESS);
    }

    NTW_INLINE constexpr bool memory::protection::readable() const noexcept
    {
        constexpr auto mask = PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_READONLY |
                              PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY |
                              PAGE_WRITECOPY;

        return _native & mask;
    }

    NTW_INLINE constexpr bool memory::protection::writable() const noexcept
    {
        constexpr auto mask = PAGE_READWRITE | PAGE_EXECUTE_READWRITE |
                              PAGE_EXECUTE_WRITECOPY | PAGE_WRITECOPY;

        return _native & mask;
    }

    NTW_INLINE constexpr bool memory::protection::executable() const noexcept
    {
        constexpr auto mask =
            PAGE_EXECUTE_READ | PAGE_EXECUTE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY;

        return _native & mask;
    }


    NTW_INLINE constexpr bool memory::protection::guarded() const noexcept
    {
        return (_native & PAGE_GUARD);
    }

    NTW_INLINE constexpr bool memory::protection::non_cached() const noexcept
    {
        return (_native & PAGE_NOCACHE);
    }

    NTW_INLINE constexpr bool memory::protection::write_combined() const noexcept
    {
        return (_native & PAGE_WRITECOMBINE);
    }


    template<class Address, class Buffer, class>
    NT_FN memory::info(Address                  addr,
                       MEMORY_INFORMATION_CLASS info_class,
                       Buffer&                  buffer,
                       SIZE_T                   buffer_size,
                       SIZE_T*                  return_size) const noexcept
    {
        return LI_NT(NtQueryVirtualMemory)(_process,
                                           address_cast<void*>(addr),
                                           info_class,
                                           &buffer,
                                           buffer_size,
                                           return_size ? return_size : &buffer_size);
    }

    template<class Address, class Callback, class... Args, class>
    NT_FN memory::info(Address                  addr,
                       MEMORY_INFORMATION_CLASS info_class,
                       Callback                 cb,
                       Args&&... args)
    {
        struct query_functor {
            memory* _thisptr;
            Address _address;
            template<class... Args>
            NT_FN operator()(Args... args) const noexcept
            {
                return _thisptr->info(_address, args...);
            }
        };
        return detail::generic_query_can_fail(
            query_functor{ this, addr }, info_class, cb, std::forward<Args>(args));
    }

    template<class Address>
    NT_FN memory::protect(Address        address,
                          SIZE_T         size,
                          unsigned long  new_prot,
                          unsigned long* old_prot) const noexcept
    {
        unsigned long old;
        return LI_NT(NtProtectVirtualMemory)(_process,
                                             address_cast<void**>(&address),
                                             &size,
                                             new_prot,
                                             old_prot ? old_prot : &old);
    }

    template<class Address>
    NT_FN memory::allocate(Address&      address,
                           SIZE_T        size,
                           unsigned long protection,
                           unsigned long type) const noexcept
    {
        return LI_NT(NtAllocateVirtualMemory)(
            _process, address_cast<void**>(&address), 0, &size, type, protection);
    }

    template<class Address>
    NT_FN memory::free(Address address, unsigned long type) const noexcept
    {
        SIZE_T freed = 0;
        return LI_NT(NtFreeVirtualMemory)(
            _process, address_cast<void**>(&address), &freed, type);
    }

    template<class Address>
    NT_FN memory::lock(Address address, SIZE_T size) const noexcept
    {
        return LI_NT(NtLockVirtualMemory)(
            _process, address_cast<void**>(&address), &size, 1);
    }

    template<class Address>
    NT_FN memory::unlock(Address address, SIZE_T size) const noexcept
    {
        return LI_NT(NtUnlockVirtualMemory)(
            _process, address_cast<void**>(&address), &size, 1);
    }

    template<class Address, class Buffer>
    NT_FN memory::read(Address address, Buffer& buffer, SIZE_T size, SIZE_T* read) const
        noexcept
    {
        return LI_NT(NtReadVirtualMemory)(
            _process, address_cast<void*>(address), ::std::addressof(buffer), size, read);
    }

    template<class Address, class Buffer>
    NT_FN memory::write(Address       address,
                        const Buffer& buffer,
                        SIZE_T        size,
                        SIZE_T*       written) const noexcept
    {
        return LI_NT(NtWriteVirtualMemory)(_process,
                                           address_cast<void*>(address),
                                           ::std::addressof(const_cast<Buffer&>(buffer)),
                                           size,
                                           written);
    }

    template<class Address, class Callback, class... Args>
    NT_FN memory::enumerate_regions(Address  begin_addr,
                                    Address  end_addr,
                                    Callback cb,
                                    Args&&... args) const
    {
        // need to cast these to avoid potential overflows
        auto        begin = address_cast<std::uintptr_t>(begin_addr);
        const auto  end   = address_cast<std::uintptr_t>(end_addr);
        region_info mem_info;

        while(begin < end) {
            const auto status = this->info(begin, MemoryBasicInformation, mem_info.get());

            if(!NT_SUCCESS(status)) {
                // if we get invalid parameter check if it's the end of address space
                if(status == STATUS_INVALID_PARAMETER) {
#ifdef _WIN64
                    if(begin >= 0x00007FFFFFFF0000)
#else
                    if(begin >= 0x7FFF0000)
#endif
                        return STATUS_SUCCESS;
                }
                return status;
            }

            begin = mem_info.end<std::uintptr_t>();

            NTW_CALLBACK_BREAK_IF_FALSE(cb, mem_info, args...);
        }

        return STATUS_SUCCESS;
    }

} // namespace ntw

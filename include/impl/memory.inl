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

namespace ntw {

    template<class T>
    NT_FN detail::unique_memory_alloc_traits<T>::allocate(SIZE_T        sz,
                                                          unsigned long protection)
    {
        auto alloc = static_cast<unique_object<unique_memory_alloc_traits<T>>*>(this);
        alloc->reset();
        return memory().allocate(*alloc->addressof(), sz, protection);
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

                ret_on_err(LI_NT(NtSetInformationProcess)(
                    NtCurrentProcess(), ProcessQuotaLimits, &qouta, sizeof(qouta)));
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

    template<class Address, class Buffer, class>
    NT_FN memory::info(Address                  addr,
                       MEMORY_INFORMATION_CLASS info_class,
                       Buffer&                  buffer,
                       SIZE_T                   buffer_size,
                       SIZE_T*                  return_size) const noexcept
    {
        assert_pointer_type<Address>();
        return LI_NT(NtQueryVirtualMemory)(_process,
                                           reinterpret_cast<void*>(addr),
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
        assert_pointer_type<Address>();
        unsigned long old;
        return LI_NT(NtProtectVirtualMemory)(_process,
                                             reinterpret_cast<void**>(&address),
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
        assert_pointer_type<Address>();
        return LI_NT(NtAllocateVirtualMemory)(
            _process, reinterpret_cast<void**>(&address), 0, &size, type, protection);
    }

    template<class Address>
    NT_FN memory::free(Address address, unsigned long type) const noexcept
    {
        assert_pointer_type<Address>();
        SIZE_T freed = 0;
        return LI_NT(NtFreeVirtualMemory)(
            _process, reinterpret_cast<void**>(&address), &freed, type);
    }

    template<class Address>
    NT_FN memory::lock(Address address, SIZE_T size) const noexcept
    {
        assert_pointer_type<Address>();
        return LI_NT(NtLockVirtualMemory)(
            _process, reinterpret_cast<void**>(&address), &size, 1);
    }

    template<class Address>
    NT_FN memory::unlock(Address address, SIZE_T size) const noexcept
    {
        assert_pointer_type<Address>();
        return LI_NT(NtUnlockVirtualMemory)(
            _process, reinterpret_cast<void**>(&address), &size, 1);
    }

} // namespace ntw
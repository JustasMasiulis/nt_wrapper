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
#include "detail/object.hpp"

namespace ntw {

    namespace detail {

        template<class T>
        struct unique_memory_alloc_traits {
            using value_type = T;

            NT_FN allocate(SIZE_T sz, unsigned long protection);

        protected:
            ~unique_memory_alloc_traits() = default;

            NTW_INLINE void close(T* ptr) const noexcept;
        };

        struct unique_memory_lock_traits {
            using value_type = void;

            template<class Address>
            NT_FN lock(Address address, SIZE_T size) noexcept;

            template<class Address>
            NT_FN lock_strong(Address address, SIZE_T size) noexcept;

        protected:
            ~unique_memory_lock_traits() = default;

            NTW_INLINE void close(void* ptr) const noexcept;

        private:
            SIZE_T _size = 0;
        };

    } // namespace detail

    class memory {
        void* _process = NtCurrentProcess();

    public:
        template<class T>
        using unique_alloc = detail::unique_object<detail::unique_memory_alloc_traits<T>>;

        using unique_lock = detail::unique_object<detail::unique_memory_lock_traits>;

        constexpr memory() = default;
        memory(void* process) noexcept : _process(process) {}

        template<class Address, class Buffer, NTW_QUERY_BUFFER_REQUIREMENT>
        NT_FN info(Address                  addr,
                   MEMORY_INFORMATION_CLASS info_class,
                   Buffer&                  buffer,
                   SIZE_T                   buffer_size = sizeof(Buffer),
                   SIZE_T*                  return_size = nullptr) const noexcept;

        template<class Address,
                 class Callback,
                 class... Args,
                 NTW_QUERY_CALLBACK_REQUIREMENT>
        NT_FN info(Address                  addr,
                   MEMORY_INFORMATION_CLASS info_class,
                   Callback                 cb,
                   Args&&... args);

        template<class Address>
        NT_FN protect(Address        address,
                      SIZE_T         size,
                      unsigned long  new_prot,
                      unsigned long* old_prot = nullptr) const noexcept;

        template<class Address>
        NT_FN allocate(Address&      address,
                       SIZE_T        size,
                       unsigned long protection,
                       unsigned long type = MEM_COMMIT | MEM_RESERVE) const noexcept;

        template<class Address>
        NT_FN free(Address address, unsigned long type = MEM_RELEASE) const noexcept;

        template<class Address>
        NT_FN lock(Address address, SIZE_T size) const noexcept;

        template<class Address>
        NT_FN unlock(Address address, SIZE_T size) const noexcept;
    };

} // namespace ntw

#include "impl/memory.inl"

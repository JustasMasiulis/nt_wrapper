#pragma once
#include "../include/resource.hpp"

namespace ntw {

    template<std::size_t ByteSize>
    NTW_INLINE constexpr void* stack_buffer<ByteSize>::data()
    {
        return &_storage;
    }

    template<std::size_t ByteSize>
    NTW_INLINE constexpr std::size_t stack_buffer<ByteSize>::size_bytes()
    {
        return ByteSize;
    }


    NTW_INLINE void* heap_alloc::_process_heap()
    {
        const auto offset = offsetof(NT_TIB, Self) * 2;
        return reinterpret_cast<const PEB*>(__readgsqword(offset))->ProcessHeap;
    }

    NTW_INLINE status heap_alloc::allocate(void** ptr, std::size_t s)
    {
        const auto allocated = NTW_IMPORT_CALL(RtlAllocateHeap)(_process_heap(), 0, s);
        if(allocated) {
            *ptr = allocated;
            return STATUS_SUCCESS;
        }
        return STATUS_NO_MEMORY;
    }

    NTW_INLINE void heap_alloc::deallocate(void* p)
    {
        NTW_IMPORT_CALL(RtlFreeHeap)(_process_heap(), 0, p);
    }


    NTW_INLINE status page_alloc::allocate(void** ptr, std::size_t sz)
    {
        void*  temp = nullptr;
        status s    = NTW_SYSCALL(NtAllocateVirtualMemory)(
            NtCurrentProcess(), &temp, 0, &sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if(s.success())
            *ptr = temp;
        return s;
    }

    NTW_INLINE void page_alloc::deallocate(void* p)
    {
        SIZE_T size = 0;
        NTW_SYSCALL(NtFreeVirtualMemory)(NtCurrentProcess(), &p, &size, MEM_RELEASE);
    }

} // namespace ntw

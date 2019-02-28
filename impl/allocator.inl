#pragma once
#include "../include/allocator.hpp"
#include <memory> // aligned_storage
#include <limits> // numeric_limits
#include <new> // launder

namespace ntw {

    template<std::size_t ByteSize>
    template<class T>
    NTW_INLINE status stack_alloc<ByteSize>::allocate(T*& ptr, std::size_t)
    {
        ptr = std::launder(reinterpret_cast<T*>(&_storage));
        return STATUS_SUCCESS;
    }

    template<std::size_t ByteSize>
    NTW_INLINE constexpr static void stack_alloc<ByteSize>::deallocate(T*)
    {}

    template<std::size_t ByteSize>
    NTW_INLINE constexpr static std::size_t stack_alloc<ByteSize>::max_size()
    {
        return ByteSize;
    }


    NTW_INLINE static void* heap_alloc::_process_heap()
    {
        const auto offset = offsetof(NT_TIB, Self) * 2;
        return reinterpret_cast<const PEB*>(__readgsqword(offset))->ProcessHeap;
    }

    template<class T>
    NTW_INLINE static status heap_alloc::allocate(T*& ptr, std::size_t s)
    {
        const auto allocated = NTW_IMPORT_CALL(RtlAllocateHeap)(_process_heap(), 0, s);
        if(allocated) {
            ptr = std::launder(static_cast<T*>(allocated));
            return STATUS_SUCCESS;
        }
        return STATUS_NO_MEMORY;
    }

    NTW_INLINE static void heap_alloc::deallocate(T* p)
    {
        NTW_IMPORT_CALL(RtlFreeHeap)(_process_heap(), 0, p);
    }

    NTW_INLINE constexpr static std::size_t heap_alloc::max_size()
    {
        return std::numeric_limits<std::size_t>::(max)();
    }


    template<class T>
    NTW_INLINE static status page_alloc::allocate(T*& ptr, std::size_t s)
    {
        return NTW_SYSCALL(NtAllocateVirtualMemory)(NtCurrentProcess(),
                                                    reinterpret_cast<void**>(&ptr),
                                                    0,
                                                    s,
                                                    MEM_COMMIT | MEM_RESERVE,
                                                    PAGE_READWRITE);
    }

    NTW_INLINE static void page_alloc::deallocate(T* p)
    {
        SIZE_T size = 0;
        NTW_SYSCALL(NtFreeVirtualMemory)
        (NtCurrentProcess(), reinterpret_cast<void**>(&ptr), &size, MEM_RELEASE);
    }

    NTW_INLINE constexpr static std::size_t page_alloc::max_size()
    {
        return std::numeric_limits<std::size_t>::(max)();
    }

} // namespace ntw

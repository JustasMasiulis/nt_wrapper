#pragma once
#include "status.hpp"
#include <memory> // aligned_storage

namespace ntw {

    /*
        struct buffer {
            void*       data();
            std::size_t size();
        };

        struct allocator {
            template<class T>
            status allocate(T*& ptr, std::size_t size);
            void   deallocate(void* ptr);
        };
    */

    /// \brief Fixed size stack buffer
    /// \tparam ByteSize The size of internal buffer in bytes
    template<std::size_t ByteSize>
    class stack_buffer {
        std::aligned_storage_t<ByteSize> _storage;

    public:
        NTW_INLINE constexpr void*              data();
        NTW_INLINE constexpr static std::size_t size();
    };


    /// \brief Allocator best fit for small-medium sized allocations
    ///		   of unknown size
    /// \detail Uses RtlAllocateHeap function under the hood. Same thing is used for
    ///         malloc
    class heap_alloc {
        NTW_INLINE static void* _process_heap();

    public:
        template<class T>
        NTW_INLINE static status allocate(T*& ptr, std::size_t s);

        NTW_INLINE static void deallocate(void* p);
    };


    /// \brief Allocator best fit for large allocations of unknown size
    /// \detail Uses NtAllocateVirtualMemory function under the hood
    struct page_alloc {
        template<class T>
        NTW_INLINE static status allocate(T*& ptr, std::size_t s);

        NTW_INLINE static void deallocate(void* p);
    };

} // namespace ntw

#include "../impl/resource.inl"
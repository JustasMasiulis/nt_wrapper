#pragma once
#include "status.hpp"

namespace ntw {

    /*
        struct allocator {
            template<class T>
            status allocate(T*& ptr, std::size_t size);

            void deallocate(T* ptr);

            std::size_t max_size();
        };
    */

    /// \brief Allocator that uses fixed size stack buffer
    /// \tparam ByteSize The size of internal buffer in bytes
    template<std::size_t ByteSize>
    class stack_alloc {
        std::aligned_storage_t<ByteSize> _storage;

    public:
        template<class T>
        NTW_INLINE status allocate(T*& ptr, std::size_t s);

        NTW_INLINE constexpr static void deallocate(T* p);

        NTW_INLINE constexpr static std::size_t max_size();
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

        NTW_INLINE static void deallocate(T* p);

        NTW_INLINE constexpr static std::size_t max_size();
    };


    /// \brief Allocator best fit for large allocations of unknown size
    /// \detail Uses NtAllocateVirtualMemory function under the hood
    struct page_alloc {
        template<class T>
        NTW_INLINE static status allocate(T*& ptr, std::size_t s);

        NTW_INLINE static void deallocate(T* p);

        NTW_INLINE constexpr static std::size_t max_size();
    };

} // namespace ntw

#include "../impl/allocator.inl"
#pragma once
#include "../resource.hpp"
#include "unwrap.hpp"

namespace ntw::detail {

    namespace {

        template<class, class = void>
        struct is_buffer : std::false_type {};

        template<class T>
        struct is_buffer<T,
                         std::void_t<decltype(std::declval<T>().data()),
                                     decltype(std::declval<T>().size())>>
            : std::true_type {};

        template<class Alloc, bool IsBuffer = is_buffer<Alloc>::value>
        struct resource_wrapper_base : Alloc {
            void*       _ptr  = nullptr;
            std::size_t _size = 0;

            NTW_INLINE constexpr void* get() const { return _ptr; }
            NTW_INLINE constexpr void* size() const { return _size; }

            NTW_INLINE status allocate(std::size_t s)
            {
                return static_cast<Alloc&>(*this).allocate(_ptr, s);
            }

            NTW_INLINE void deallocate() const
            {
                static_cast<Alloc&>(*this).deallocate(_ptr);
            }
        };

        template<class Buffer>
        struct resource_wrapper_base<Buffer, true> : Buffer {
            using Buffer::data;
            using Buffer::size;

            NTW_INLINE status allocate(std::size_t s)
            {
                return s > static_cast<Alloc&>(*this).size() ? STATUS_NO_MEMORY
                                                             : STATUS_SUCCESS;
            }

            NTW_INLINE constexpr static void deallocate() {}
        };

    } // namespace


    template<class Resource>
    class resource_wrapper {
        resource_wrapper_base<Resource> _base;

        NTW_INLINE void _dealloc()
        {
            if(_base.get())
                _base.deallocate();
        }

    public:
        NTW_INLINE ~resource_wrapper() { _dealloc(); }

        NTW_INLINE void* get() const { return _base.get(); }

        NTW_INLINE std::size_t size() const { return _base.size(); }

        NTW_INLINE status expand(std::size_t s)
        {
            _dealloc();
            return _base.allocate(s);
        }
    };

} // namespace ntw::detail
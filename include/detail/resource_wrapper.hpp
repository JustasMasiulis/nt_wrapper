#pragma once
#include "../resource.hpp"
#include "unwrap.hpp"

namespace ntw::detail {

    template<class, class = void>
    struct is_buffer : std::false_type {};

    template<class T>
    struct is_buffer<T,
                     std::void_t<decltype(std::declval<T>().data()),
                                 decltype(std::declval<T>().size_bytes())>>
        : std::true_type {};

    namespace {

        template<class Alloc>
        class resource_wrapper_alloc_base : Alloc {
            void*       _ptr  = nullptr;
            std::size_t _size = 0;

        public:
            using Alloc::deallocate;

            NTW_INLINE constexpr void* data() const { return _ptr; }
            NTW_INLINE constexpr void* size_bytes() const { return _size; }

            NTW_INLINE status allocate(std::size_t s)
            {
                const auto status = static_cast<Alloc&>(*this).allocate(_ptr, s);
                if(status.success())
                    _size = s;

                return status;
            }
        };

        template<class Buffer>
        struct resource_wrapper_buffer_base : Buffer {
            using Buffer::data;
            using Buffer::size_bytes;

            NTW_INLINE status allocate(std::size_t s)
            {
                return s > size_bytes() ? STATUS_NO_MEMORY : STATUS_SUCCESS;
            }

            NTW_INLINE constexpr static void deallocate(void*) {}
        };

        template<class Resource>
        using resource_wrapper_base =
            std::conditional_t<is_buffer<Resource>::value,
                               resource_wrapper_buffer_base<Resource>,
                               resource_wrapper_alloc_base<Resource>>;

    } // namespace


    template<class Resource>
    class resource_wrapper : resource_wrapper_base<Resource> {
        using base_type = resource_wrapper_base<Resource>;

    public:
        using base_type::data;
        using base_type::size_bytes;
        using base_type::allocate;
        using base_type::deallocate;

        NTW_INLINE ~resource_wrapper()
        {
            if(const auto ptr = data(); ptr)
                deallocate(ptr);
        }

        NTW_INLINE status realloc(std::size_t s)
        {
            const auto ptr    = data();
            const auto status = allocate(s);
            if(ptr && status.success())
                deallocate(ptr);
            return status;
        }
    };

} // namespace ntw::detail
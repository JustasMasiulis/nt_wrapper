#pragma once
#include "../detail/common.hpp"

namespace ntw::sys {

    class loaded_module {
        std::uint16_t offset_to_next;
        void*         section;
        void*         mapped_base;
        void*         image_base;
        ulong_t       image_size;
        ulong_t       flags;
        std::uint16_t load_order_idx;
        std::uint16_t init_order_idx;
        std::uint16_t load_count;
        std::uint16_t file_name_offset;
        char          path[256];
        ulong_t       image_checksum;
        ulong_t       timestamp;
        void*         default_base;

        /// \brief Returns a null terminated file name string.
        NTW_INLINE const char* name() const noexcept { return path + file_name_offset; }

        /// \brief Returns string_view which contains the file name
        NTW_INLINE std::string_view name_view() const noexcept
        {
            const auto first = name();
            auto       last  = first;
            while(*last)
                ++last;

            return std::string_view(
                first, static_cast<std::string_view::size_type>(last - first));
        }

        NTW_INLINE std::string_view path_view() const noexcept {
        
        }
    };

} // namespace ntw::sys
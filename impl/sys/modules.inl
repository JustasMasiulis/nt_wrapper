#pragma once
#include "../../include/ntw/sys/modules.hpp"

namespace ntw::sys {

    NTW_INLINE const char* loaded_module::name() const noexcept
    {
        return path + file_name_offset;
    }

    NTW_INLINE std::string_view loaded_module::name_view() const noexcept
    {
        const auto first = name();
        auto       last  = first;
        while(*last)
            ++last;

        return std::string_view(first,
                                static_cast<std::string_view::size_type>(last - first));
    }

    NTW_INLINE std::string_view loaded_module::path_view() const noexcept
    {
        auto driver_name = name_view();
        return { driver_name.data() - file_name_offset,
                 driver_name.size() + file_name_offset };
    }

    template<class Range>
    ntw::result<loaded_module::range_type> loaded_modules(Range&& buffer)
    {
        const auto  first  = detail::unfancy(detail::adl_begin(buffer));
        const auto  size   = static_cast<ulong_t>(detail::range_byte_size(buffer));
        ntw::status status = NTW_SYSCALL(NtQuerySystemInformation)(
            SystemModuleInformationEx, first, size, nullptr);

        return { status, { reinterpret_cast<loaded_module*>(first) } };
    }

} // namespace ntw::sys
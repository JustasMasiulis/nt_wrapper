#pragma once
#include "../../include/ntw/vm/protection.hpp"

namespace ntw::vm {

    NTW_INLINE constexpr protection::protection(std::uint32_t raw_value) noexcept
        : value(raw_value){};

    NTW_INLINE constexpr std::uint32_t protection::get() const noexcept { return value; }

    NTW_INLINE constexpr bool protection::accessible() const noexcept
    {
        return !(value & PAGE_NOACCESS);
    }

    NTW_INLINE constexpr bool protection::readable() const noexcept
    {
        return value &
               (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
                PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
    }

    NTW_INLINE constexpr bool protection::writeable() const noexcept
    {
        return value & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE |
                        PAGE_EXECUTE_WRITECOPY);
    }

    NTW_INLINE constexpr bool protection::executeable() const noexcept
    {
        return value & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                        PAGE_EXECUTE_WRITECOPY);
    }

    NTW_INLINE constexpr bool protection::guarded() const noexcept
    {
        return value & PAGE_GUARD;
    }

    NTW_INLINE constexpr bool protection::non_cached() const noexcept
    {
        return value & PAGE_NOCACHE;
    }

    NTW_INLINE constexpr bool protection::write_combined() const noexcept
    {
        return value & PAGE_WRITECOMBINE;
    }

    NTW_INLINE constexpr protection protection::no_access() noexcept
    {
        return { PAGE_NOACCESS };
    }

    NTW_INLINE constexpr protection protection::read() noexcept
    {
        return { PAGE_READONLY };
    }

    NTW_INLINE constexpr protection protection::execute() noexcept
    {
        return { PAGE_EXECUTE };
    }

    NTW_INLINE constexpr protection protection::read_write() noexcept
    {
        return { PAGE_READWRITE };
    }

    NTW_INLINE constexpr protection protection::read_writecopy() noexcept
    {
        return { PAGE_WRITECOPY };
    }

    NTW_INLINE constexpr protection protection::read_execute() noexcept
    {
        return { PAGE_EXECUTE_READ };
    }

    NTW_INLINE constexpr protection protection::read_write_execute() noexcept
    {
        return { PAGE_EXECUTE_READWRITE };
    }

    NTW_INLINE constexpr protection protection::read_writecopy_execute() noexcept
    {
        return { PAGE_EXECUTE_WRITECOPY };
    }

    NTW_INLINE constexpr protection& protection::guard() noexcept
    {
        value |= PAGE_GUARD;
        return *this;
    }

    NTW_INLINE constexpr protection& protection::disable_caching() noexcept
    {
        value |= PAGE_NOCACHE;
        return *this;
    }

    NTW_INLINE constexpr protection& protection::combine_writes() noexcept
    {
        value |= PAGE_NOCACHE;
        return *this;
    }

} // namespace ntw::vm

#pragma once
#include "../include/ntw/unicode_string.hpp"

namespace ntw {

    NTW_INLINE constexpr unicode_string::unicode_string() : unicode_string(nullptr, 0) {}

    template<std::size_t N>
    NTW_INLINE constexpr unicode_string::unicode_string(const wchar_t (&str)[N])
        : _value{ static_cast<std::uint16_t>((N - 1) << 1),
                  static_cast<std::uint16_t>((N - 1) << 1),
                  const_cast<wchar_t*>(str) }
    {}

    NTW_INLINE constexpr unicode_string::unicode_string(UNICODE_STRING str) : _value(str)
    {}

    NTW_INLINE constexpr unicode_string::unicode_string(const wchar_t* str,
                                                        std::uint16_t  len)
        : _value{ static_cast<std::uint16_t>(len << 1),
                  static_cast<std::uint16_t>(len << 1),
                  const_cast<wchar_t*>(str) }
    {}

    NTW_INLINE constexpr unicode_string::unicode_string(std::wstring_view view)
        : unicode_string(view.data(), static_cast<std::uint16_t>(view.size()))
    {}

    NTW_INLINE UNICODE_STRING& unicode_string::get() { return _value; }
    NTW_INLINE const UNICODE_STRING& unicode_string::get() const { return _value; }

    NTW_INLINE wchar_t* unicode_string::begin() { return _value.Buffer; }
    NTW_INLINE const wchar_t* unicode_string::begin() const { return _value.Buffer; }

    NTW_INLINE unicode_string::reverse_iterator unicode_string::rbegin()
    {
        return reverse_iterator{ end() };
    }
    NTW_INLINE unicode_string::const_reverse_iterator unicode_string::rbegin() const
    {
        return const_reverse_iterator{ end() };
    }

    NTW_INLINE wchar_t* unicode_string::end()
    {
        return _value.Buffer + (_value.Length >> 1);
    }
    NTW_INLINE const wchar_t* unicode_string::end() const
    {
        return _value.Buffer + (_value.Length >> 1);
    }

    NTW_INLINE unicode_string::reverse_iterator unicode_string::rend()
    {
        return reverse_iterator{ begin() };
    }
    NTW_INLINE unicode_string::const_reverse_iterator unicode_string::rend() const
    {
        return const_reverse_iterator{ begin() };
    }

    NTW_INLINE bool unicode_string::empty() const { return _value.Length == 0; }

    NTW_INLINE std::uint16_t unicode_string::byte_size() const { return _value.Length; }
    NTW_INLINE std::uint16_t unicode_string::size() const { return _value.Length >> 1; }

    NTW_INLINE std::wstring_view unicode_string::view() const
    {
        return std::wstring_view{ _value.Buffer,
                                  static_cast<std::size_t>(_value.Length) >> 1u };
    }

} // namespace ntw
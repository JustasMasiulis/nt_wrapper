#pragma once
#include "../include/ntw/unicode_string.hpp"

namespace ntw {

    /// \brief Constructrs an empty string
    NTW_INLINE constexpr unicode_string::unicode_string() : unicode_string(nullptr, 0) {}

    /// \brief Constructrs an empty string
    NTW_INLINE constexpr unicode_string::unicode_string(UNICODE_STRING str) : _value(str)
    {}

    /// \brief Constructs unicode_string out of wide string and its length
    /// \param len The length of given string in characters
    NTW_INLINE constexpr unicode_string::unicode_string(const wchar_t* str,
                                                        std::uint16_t  len)
        : _value{ static_cast<std::uint16_t>(len << 1),
                  static_cast<std::uint16_t>(len << 1),
                  const_cast<wchar_t*>(str) }
    {}

    /// \brief Constructs unicode_string out of a view
    NTW_INLINE constexpr unicode_string::unicode_string(std::wstring_view view)
        : unicode_string(view.data(), static_cast<std::uint16_t>(view.size()))
    {}

    /// \brief Returns the internal UNICODE_STRING
    NTW_INLINE UNICODE_STRING& unicode_string::get() { return _value; }
    /// \brief Returns the internal UNICODE_STRING
    NTW_INLINE const UNICODE_STRING& unicode_string::get() const { return _value; }

    /// \brief Returns the beginning of buffer
    NTW_INLINE wchar_t* unicode_string::begin() { return _value.Buffer; }
    /// \brief Returns the beginning of buffer
    NTW_INLINE const wchar_t* unicode_string::begin() const { return _value.Buffer; }

    /// \brief Returns one past the end of buffer
    NTW_INLINE wchar_t* unicode_string::end()
    {
        return _value.Buffer + (_value.Length >> 1);
    }
    /// \brief Returns one past the end of buffer
    NTW_INLINE const wchar_t* unicode_string::end() const
    {
        return _value.Buffer + (_value.Length >> 1);
    }

    NTW_INLINE bool unicode_string::empty() const { return _value.Length == 0; }

    /// \brief Returns the size of string in characters
    NTW_INLINE std::uint16_t unicode_string::size() const { return _value.Length >> 1; }

    /// \brief Returns a view of the internal string
    NTW_INLINE std::wstring_view unicode_string::view() const
    {
        return std::wstring_view{ _value.Buffer,
                                  static_cast<std::size_t>(_value.Length) >> 1u };
    }

} // namespace ntw
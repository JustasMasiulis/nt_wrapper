#pragma once
#include "detail/common.hpp"
#include <string_view>
#include <cstdint>

namespace ntw {

    class unicode_string {
        UNICODE_STRING _value;

    public:
        /// \brief Constructrs an empty string
        NTW_INLINE constexpr unicode_string();

        /// \brief Constructs unicode_string when given a C string expression
        template<std::size_t N>
        NTW_INLINE constexpr unicode_string(const wchar_t (&str)[N]);

        /// \brief Constructrs unicode_string using the given string
        NTW_INLINE constexpr unicode_string(UNICODE_STRING str);

        /// \brief Constructs unicode_string out of wide string and its length
        /// \param len The length of given string in characters
        NTW_INLINE constexpr unicode_string(const wchar_t* str, std::uint16_t len);

        /// \brief Constructs unicode_string out of a view
        NTW_INLINE constexpr unicode_string(std::wstring_view view);

        /// \brief Returns the internal UNICODE_STRING
        NTW_INLINE UNICODE_STRING& get();

        /// \brief Returns the internal UNICODE_STRING
        NTW_INLINE const UNICODE_STRING& get() const;

        /// \brief Returns the beginning of buffer
        NTW_INLINE wchar_t* begin();

        /// \brief Returns the beginning of buffer
        NTW_INLINE const wchar_t* begin() const;

        /// \brief Returns one past the end of buffer
        NTW_INLINE wchar_t* end();

        /// \brief Returns one past the end of buffer
        NTW_INLINE const wchar_t* end() const;

        NTW_INLINE bool empty() const;

        /// \brief Returns the size of string in characters
        NTW_INLINE std::uint16_t size() const;

        /// \brief Returns a view of the internal string
        NTW_INLINE std::wstring_view view() const;
    };

} // namespace ntw

#include "../impl/unicode_string.inl"
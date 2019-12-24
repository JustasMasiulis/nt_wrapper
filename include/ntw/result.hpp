#pragma once
#include <type_traits>
#include "status.hpp"

namespace ntw {

    template<class T>
    class result final : public ntw::status {
        using base_type = ntw::status;
        T _value;

    public:
        NTW_INLINE constexpr result() = default;

        NTW_INLINE constexpr result(const result&)  = default;
        NTW_INLINE constexpr result(result&& other) = default;

        NTW_INLINE constexpr result& operator=(const result&) = default;
        NTW_INLINE constexpr result& operator=(result&& other) = default;

        NTW_INLINE constexpr result(status s) : base_type(s) {}
        NTW_INLINE constexpr result(status s, T&& v) : base_type(s), _value(std::move(v))
        {}
        NTW_INLINE constexpr result(status s, const T& v) : base_type(s), _value(v) {}

        NTW_INLINE constexpr status&            status() noexcept;
        NTW_INLINE constexpr const ntw::status& status() const noexcept;

        NTW_INLINE constexpr const T& operator*() const noexcept;
        NTW_INLINE constexpr T&       operator*() noexcept;

        NTW_INLINE constexpr const T* operator->() const noexcept;
        NTW_INLINE constexpr T*       operator->() noexcept;

        NTW_INLINE constexpr operator bool() const noexcept { return success(); }

        template<class Fn>
        NTW_INLINE constexpr auto then(Fn fn)->ntw::result<decltype(fn(**this))>;
    };

    template<class T>
    class result_ref final : public status {
        using base_type = ntw::status;
        T* _value       = nullptr;

    public:
        NTW_INLINE constexpr result_ref() = default;

        template<class U>
        NTW_INLINE constexpr result_ref(status s, T* v) : base_type(s), _value(v)
        {}

        NTW_INLINE constexpr status&            status() noexcept;
        NTW_INLINE constexpr const ntw::status& status() const noexcept;

        NTW_INLINE constexpr std::add_lvalue_reference_t<T> operator*() const;
        NTW_INLINE constexpr T*                             operator->() const noexcept;
    };

} // namespace ntw

#include "../impl/result.inl"
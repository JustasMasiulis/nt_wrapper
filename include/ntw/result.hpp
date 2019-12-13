#pragma once
#include <type_traits>
#include "status.hpp"

namespace ntw {

    template<class T>
    class result final : public status {
        static_assert(std::is_trivial_v<T>, "ntw::result expect a trivial type");
        T _value;

    public:
        constexpr result() = default;

        template<class U>
        constexpr result(const U& other) : _value(other)
        {}

        template<class U>
        constexpr result(U&& other) : _value(std::forward<U>(other))
        {}

        constexpr status&       status() noexcept;
        constexpr const ntw::status& status() const noexcept;

        constexpr std::add_lvalue_reference_t<T> operator*() const;
        constexpr T*                             operator->() const noexcept;
    };

    template<class T>
    class result_ref final : public status {
        static_assert(std::is_trivial_v<T>, "ntw::result expect a trivial type");
        T* _value = nullptr;

    public:
        constexpr result_ref() = default;

        constexpr result_ref(const T* other) noexcept : _value(other) {}

        constexpr status&       status() noexcept;
        constexpr const ntw::status& status() const noexcept;

        constexpr std::add_lvalue_reference_t<T> operator*() const;
        constexpr T*                             operator->() const noexcept;
    };

} // namespace ntw

#include "../impl/result.inl"
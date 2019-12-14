#pragma once
#include <type_traits>
#include "status.hpp"

namespace ntw {

    template<class T>
    class result final : public ntw::status {
        using base_type = ntw::status;
        static_assert(std::is_trivially_copyable<T>::value,
                      "ntw::result expect a trivially copyable type");
        T _value;

    public:
        constexpr result() = default;

        template<class U>
        constexpr result(status s, U&& v) : base_type(s), _value(std::forward<U>(v))
        {}

        constexpr status&            status() noexcept;
        constexpr const ntw::status& status() const noexcept;

        constexpr const T& operator*() const noexcept;
        constexpr T&       operator*() noexcept;

        constexpr const T* operator->() const noexcept;
        constexpr T*       operator->() noexcept;
    };

    template<class T>
    class result_ref final : public status {
        using base_type = ntw::status;
        static_assert(std::is_trivial_v<T>, "ntw::result expect a trivial type");
        T* _value = nullptr;

    public:
        constexpr result_ref() = default;

        template<class U>
        constexpr result_ref(status s, T* v) : base_type(s), _value(v)
        {}

        constexpr status&            status() noexcept;
        constexpr const ntw::status& status() const noexcept;

        constexpr std::add_lvalue_reference_t<T> operator*() const;
        constexpr T*                             operator->() const noexcept;
    };

} // namespace ntw

#include "../impl/result.inl"
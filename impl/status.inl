#pragma once
#include "../include/status.hpp"
#include <type_traits>

namespace ntw {

    status::status() : _value(0) {}

    status::status(std::int32_t status) : _value(status) {}

    status& status::operator=(std::int32_t status)
    {
        _value = status;
        return *this;
    }

    bool status::operator==(std::int32_t status) const { return _value == status; }

    status::operator bool() const { return _value >= 0; }

    status::operator std::int32_t() const { return _value; }

    bool status::success() const { return _value >= 0; }

    bool status::information() const
    {
        return (static_cast<std::uint32_t>(_value) >> 30) == 1;
    }

    bool status::warning() const
    {
        return (static_cast<std::uint32_t>(_value) >> 30) == 2;
    }

    bool status::error() const { return (static_cast<std::uint32_t>(_value) >> 30) == 3; }


    severity status::severity() const
    {
        return static_cast<ntw::severity>(static_cast<std::uint32_t>(_value) >> 30);
    }

    facility status::facility() const
    {
        return static_cast<ntw::facility>((static_cast<std::uint32_t>(_value) << 4) >>
                                          20);
    }

    std::int32_t status::code() const
    {
        return static_cast<std::uint32_t>(_value) & 0xFFFF;
    }

    std::int32_t status::get() const { return _value; }

    template<class Fn>
    status& status::and_then(Fn f)
    {
        if(_value >= 0) {
            if constexpr(std::is_same<decltype(f(*this)), void>::value)
                f(*this);
            else
                _value = f(*this);
        }
        return *this;
    }

    template<class Fn>
    status& status::or_else(Fn f)
    {
        if(_value < 0) {
            if constexpr(std::is_same<decltype(f(*this)), void>::value)
                f(*this);
            else
                _value = f(*this);
        }
        return *this;
    }

} // namespace ntw
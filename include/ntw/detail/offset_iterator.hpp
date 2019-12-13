#pragma once
#include "macros.hpp"
#include <cstddef>
#include <iterator>

namespace ntw::detail {

    template<class Traits>
    class offset_iterator {
        typename Traits::value_type* _info = nullptr;

    public:
        using difference_type   = std::ptrdiff_t;
        using value_type        = typename Traits::value_type;
        using pointer           = typename Traits::value_type*;
        using reference         = typename Traits::value_type&;
        using iterator_category = std::input_iterator_tag;

        NTW_INLINE constexpr offset_iterator() noexcept = default;

        NTW_INLINE constexpr offset_iterator(pointer info) noexcept : _info(info) {}

        NTW_INLINE reference operator*() const noexcept
        {
            return *reinterpret_cast<pointer>(_info);
        }

        NTW_INLINE pointer operator->() const noexcept
        {
            return reinterpret_cast<pointer>(_info);
        }

        NTW_INLINE offset_iterator& operator++() noexcept
        {
            const auto offset = Traits::next_entry_offset(_info);
            if(_info && offset != 0)
                _info =
                    reinterpret_cast<pointer>(reinterpret_cast<char*>(_info) + offset);
            else
                _info = nullptr;

            return *this;
        }

        NTW_INLINE offset_iterator operator++(int) noexcept
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        NTW_INLINE bool _is_equal(const offset_iterator& other) const noexcept
        {
            return _info == other._info;
        }
    };

    template<class T>
    NTW_INLINE bool operator==(offset_iterator<T> lhs, offset_iterator<T> rhs) noexcept
    {
        return lhs._is_equal(rhs);
    }

    template<class T>
    NTW_INLINE bool operator!=(offset_iterator<T> lhs, offset_iterator<T> rhs) noexcept
    {
        return !lhs._is_equal(rhs);
    }

} // namespace ntw::detail
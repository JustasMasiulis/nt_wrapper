/*
 * Copyright 2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "../detail/common.hpp"
#include "../detail/unwrap.hpp"
#include <cstdint>

namespace ntw::se::ace {

    enum class type : std::uint8_t {
        access_allowed,
        access_denied,
        system_audit,
        system_alarm,

        access_allowed_compound,

        access_allowed_object,
        access_denied_object,
        access_audit_object,
        access_alarm_object,
        access_allowed_callback,
        access_denied_callback,
        access_allowed_object_callback,
        access_denied_object_callback,
        access_audit_callback,
        access_alarm_callback,
        access_audit_object_callback,
        access_alarm_object_callback,

        mandatory_label,
        resource_attribute,
        scoped_policy_id,
        process_trust_label,
        access_filter
    };


    struct header {
        ::ntw::se::ace::type type;
        std::uint8_t         flags = 0;
        std::int16_t         size;

        NTW_INLINE constexpr header(::ntw::se::ace::type typ, std::int16_t sz) noexcept
            : type(typ), size(sz)
        {}
    };

    template<class Derived>
    class ace_flags {
    protected:
        NTW_INLINE constexpr Derived& _toggle(bool enable, std::uint8_t flag) noexcept
        {
            if(enable)
                static_cast<Derived&>(*this).header.flags |= flag;
            else
                static_cast<Derived&>(*this).header.flags &= ~flag;
            return *this;
        }

    public:
        NTW_INLINE constexpr bool object_inherit() noexcept
        {
            return static_cast<const Derived&>(*this).header.flags & OBJECT_INHERIT_ACE;
        }

        NTW_INLINE constexpr Derived& object_inherit(bool enable = true) noexcept
        {
            return _toggle(enable, OBJECT_INHERIT_ACE);
        }


        NTW_INLINE constexpr bool container_inherit() const noexcept
        {
            return static_cast<const Derived&>(*this).header.flags &
                   CONTAINER_INHERIT_ACE;
        }

        NTW_INLINE constexpr Derived& container_inherit(bool enable = true) noexcept
        {
            return _toggle(enable, CONTAINER_INHERIT_ACE);
        }


        NTW_INLINE constexpr bool no_propagate_inherit() const noexcept
        {
            return static_cast<const Derived&>(*this).header.flags &
                   NO_PROPAGATE_INHERIT_ACE;
        }

        NTW_INLINE constexpr Derived& no_propagate_inherit(bool enable = true) noexcept
        {
            return _toggle(enable, NO_PROPAGATE_INHERIT_ACE);
        }


        NTW_INLINE constexpr bool inherit_only() const noexcept
        {
            return static_cast<const Derived&>(*this).header.flags & INHERIT_ONLY_ACE;
        }

        NTW_INLINE constexpr Derived& inherit_only(bool enable = true) noexcept
        {
            return _toggle(enable, INHERIT_ONLY_ACE);
        }

        NTW_INLINE bool inherited() const noexcept
        {
            return static_cast<const Derived&>(*this).header.flags & INHERITED_ACE;
        }
    };

    template<class SidT>
    struct access_allowed : ace_flags<access_allowed<SidT>> {
        ::ntw::se::ace::header header;
        ACCESS_MASK            mask;
        SidT                   sid;

        NTW_INLINE constexpr access_allowed() noexcept
            : header(type::access_allowed, sizeof(*this))
        {}

        NTW_INLINE constexpr access_allowed(ACCESS_MASK access_mask, SidT sid_) noexcept
            : header(type::access_allowed, sizeof(*this)), mask(access_mask), sid(sid_)
        {}

        NTW_INLINE constexpr bool critical() const noexcept
        {
            return header.flags & CRITICAL_ACE_FLAG;
        }

        NTW_INLINE constexpr access_allowed& critical(bool enable = true) noexcept
        {
            return this->_toggle(enable, CRITICAL_ACE_FLAG);
        }
    };

    template<class SidT>
    struct access_denied : ace_flags<access_denied<SidT>> {
        ::ntw::se::ace::header header;
        ACCESS_MASK            mask;
        SidT                   sid;

        NTW_INLINE access_denied() : header(type::access_denied, sizeof(*this)) {}

        NTW_INLINE constexpr access_denied(ACCESS_MASK access_mask, SidT sid_) noexcept
            : header(type::access_denied, sizeof(*this)), mask(access_mask), sid(sid_)
        {}
    };

} // namespace ntw::se::ace

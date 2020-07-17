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

namespace ntw::se {

    template<class... Aces>
    struct static_acl {
        ACL                 acl;
        std::tuple<Aces...> aces;

        NTW_INLINE constexpr static_acl() noexcept
            : acl{ ACL_REVISION, 0, sizeof(static_acl<Aces...>) + 1, sizeof...(Aces), 0 }
        {}

        NTW_INLINE constexpr static_acl(Aces... aces_) noexcept
            : acl{ ACL_REVISION, 0, sizeof(static_acl<Aces...>) + 1, sizeof...(Aces), 0 }
            , aces(aces_...)
        {}

        operator ACL*() { return reinterpret_cast<ACL*>(this); }
    };

    template<class... Aces>
    static_acl(Aces...) -> static_acl<Aces...>;


    struct dyn_acl {
        ACL acl;
    };

} // namespace ntw::se

#pragma once
#include "../detail/common.hpp"
#include "../detail/unwrap.hpp"
#include <cstdint>

namespace ntw::se {

  struct ace{
        ACCESS_DENIED_ACE ace;
    }

    template<std::size_t NumAces>
    class static_acl {
        ACL _acl;
    };

} // namespace ntw::se

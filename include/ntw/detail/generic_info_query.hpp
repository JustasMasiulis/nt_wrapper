#pragma once
#include "../status.hpp"
#include "unwrap.hpp"

namespace ntw::detail {

    template<class Size = ulong_t, class Resource, class Obj, class... Args>
    status generic_info_query(Resource& res, const Obj& obj, Args... args)
    {
        Size   required;
        status s;
        while(true) {
            auto s = obj.info(args..., &required);

            if(s == STATUS_INFO_LENGTH_MISMATCH || s == STATUS_BUFFER_TOO_SMALL) {
                // attempt to expand buffer
                s = res.expand(required);

                if(!s.success())
                    return s; // expand failed
            }
            // success or not return the status
            else
                return s;
        }
    }

} // namespace ntw::detail
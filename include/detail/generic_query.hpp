/*
 * Copyright 2018 Justas Masiulis
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
#include "../memory.hpp"

#define NTW_IMPLEMENT_QUERY_CALLBACK                         \
    ntw::detail::generic_query_can_fail(                     \
        [&](auto&&... args) { return this->info(args...); }, \
        cb,                                                  \
        std::forward<Args>(args)...);

namespace ntw::detail {

    template<class QueryFunction, class InfoClass, class T>
    NT_FN generic_query_can_fail(QueryFunction            query,
                                 InfoClass                info_class,
                                 memory::unique_alloc<T>& buffer,
                                 unsigned long&           size) noexcept
    {
        // get the size first
        auto status = query(info_class, buffer.get(), size, &size);

        if(status == STATUS_INFO_LENGTH_MISMATCH || status == STATUS_BUFFER_TOO_SMALL) {
            // add an extra page for more information
            size += 0x1000;
            ret_on_err(buffer.allocate(size, PAGE_READWRITE));
            status = query(info_class, buffer.get(), size, &size);
        }

        return status;
    }

    template<class QueryFunction, class InfoClass, class Callback, class... Args>
    NT_FN generic_query_can_fail(QueryFunction query,
                                 InfoClass     info_class,
                                 Callback&     cb,
                                 Args&&... args)
    {
        memory::unique_alloc<void> buffer;
        unsigned long              size = 0;

        const auto status = generic_query_can_fail(query, info_class, buffer, size);
        if(NT_SUCCESS(status))
            cb(buffer.get(), size, std::forward<Args>(args)...);

        return status;
    }

} // namespace ntw::detail
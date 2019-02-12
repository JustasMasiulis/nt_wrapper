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
#include "../base_file.hpp"

namespace ntw::io::traits {

    template<class Handle, bool Synchronous>
    struct basic_directory_traits {
        using handle_type  = Handle;
        using options_type = file_options;

        constexpr static auto options = options_type{}.share_all().full_access();

        NT_FN static open(void*&              handle,
                          OBJECT_ATTRIBUTES&  attributes,
                          const options_type& options,
                          unsigned long       disposition);
    };

    template<class Handle>
    using directory_traits = basic_directory_traits<Handle, true>;

    template<class Handle>
    using async_directory_traits = basic_directory_traits<Handle, false>;

    template<class Handle, bool Sync>
    NT_FN basic_directory_traits<Handle, Sync>::open(void*&              handle,
                                                     OBJECT_ATTRIBUTES&  attributes,
                                                     const options_type& options,
                                                     unsigned long       disposition)
    {
        IO_STATUS_BLOCK status_block;
        return LI_NT(NtCreateFile)(&handle,
                                   detail::synchronize_access<Sync>(options),
                                   &attributes,
                                   &status_block,
                                   nullptr,
                                   detail::normalize_attributes(options),
                                   options.data().share_access,
                                   disposition,
                                   detail::synchronize_options<Sync>(options) |
                                       FILE_DIRECTORY_FILE,
                                   nullptr,
                                   0);
    };

} // namespace ntw::io::traits

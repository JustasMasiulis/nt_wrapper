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
#include "../result.hpp"
#include "../unicode_string.hpp"
#include "../io/registry_key.hpp"

namespace ntw::sys {

    NTW_INLINE status load_driver(unicode_string service_name);

    NTW_INLINE status unload_driver(unicode_string service_name);

} // namespace ntw::sys

#include "impl/driver_loader.inl"
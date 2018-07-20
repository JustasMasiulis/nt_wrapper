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
#include "../native_peb.hpp"

TEB& ntw::local_teb() noexcept
{
    return *reinterpret_cast<TEB*>(
#ifdef _M_X64

        __readgsqword(offsetof(NT_TIB, Self))

#elif _M_IX86

        __readfsdword(offsetof(NT_TIB, Self))

#elif _M_ARM

        _MoveFromCoprocessor(15, 0, 13, 0, 2)

#elif _M_ARM64

        __getReg(18)

#else

#error "architecture unsupported"

#endif
    );
}

PEB& ntw::local_peb() noexcept { return *local_teb().ProcessEnvironmentBlock; }

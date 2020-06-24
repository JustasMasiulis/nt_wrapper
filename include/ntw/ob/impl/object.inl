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
#include "../object.hpp"

namespace ntw::ob {

    template<class S>
    inline constexpr NTW_INLINE const S& basic_object<S>::storage() const
    {
        return *this;
    }

    template<class S>
    inline constexpr NTW_INLINE S& basic_object<S>::storage()
    {
        return *this;
    }

    template<class S>
    NTW_INLINE constexpr void basic_object<S>::reset(void* new_handle) noexcept
    {
        *static_cast<S*>(this) = S{ new_handle };
    }

    template<class S>
    NTW_INLINE constexpr void basic_object<S>::reset() noexcept
    {
        *static_cast<S*>(this) = nullptr;
    }

    template<class S>
    NTW_INLINE constexpr basic_object<S>::operator bool() const
    {
        return !!get();
    }


    template<class S>
    NTW_INLINE status basic_object<S>::make_temporary() const
    {
        return NTW_SYSCALL(NtMakeTemporaryObject)(get());
    }

    template<class S>
    NTW_INLINE status basic_object<S>::make_permanent() const
    {
        return NTW_SYSCALL(NtMakePermanentObject)(get());
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait() const
    {
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), false, nullptr);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait(alertable_t) const
    {
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), true, nullptr);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait_for(duration timeout) const
    {
        LARGE_INTEGER li;
        li.QuadPart = -timeout.count();
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), false, &li);
    }

    template<class S>
    NTW_INLINE status basic_object<S>::wait_for(duration timeout, alertable_t) const
    {
        LARGE_INTEGER li;
        li.QuadPart = -timeout.count();
        return NTW_SYSCALL(NtWaitForSingleObject)(get(), true, &li);
    }

} // namespace ntw::ob
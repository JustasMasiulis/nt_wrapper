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

namespace ntw::io {

    template<class Handle>
    class basic_transaction {
        Handle _handle;

    public:
        using handle_type              = Handle;

        NTW_INLINE basic_transaction() = default;

        template<class HandleType>
        NTW_INLINE basic_transaction(const HandleType& handle) noexcept
            : _handle(unwrap_handle(handle))
        {}

        NTW_INLINE handle_type& handle() noexcept { return _handle; }
        NTW_INLINE const handle_type& handle() const noexcept { return _handle; }

        NT_FN create(ACCESS_MASK    access,
                     LARGE_INTEGER* timeout         = nullptr,
                     bool           allow_promotion = true)
        {
            void*      temp;
            const auto status = LI_NT(NtCreateTransaction)(
                &temp,
                access,
                nullptr,
                nullptr,
				nullptr,
                allow_promotion ? 0 : TRANSACTION_DO_NOT_PROMOTE,
                0,
                0,
                timeout,
                nullptr);
            if(NT_SUCCESS(status))
                _handle.reset(temp);

            return status;
        }

        NT_FN commit() const noexcept
        {
            return LI_NT(NtCommitTransaction)(_handle.get(), true);
        }

        NT_FN commit_async() const noexcept
        {
            return LI_NT(NtCommitTransaction)(_handle.get(), false);
        }

        NT_FN rollback() const noexcept
        {
            return LI_NT(NtRollbackTransaction)(_handle.get(), true);
        }

        NT_FN rollback_async() const noexcept
        {
            return LI_NT(NtRollbackTransaction)(_handle.get(), false);
        }
    };

    class transaction_guard {
        void* _handle;

    public:
        template<class Handle>
        NTW_INLINE transaction_guard(const Handle& handle) noexcept
            : _handle(std::exchange(local_teb().CurrentTransactionHandle,
                                    unwrap_handle(handle)))
        {}

        NTW_INLINE ~transaction_guard() noexcept
        {
            local_teb().CurrentTransactionHandle = _handle;
        }
    };

    using unique_transaction = basic_transaction<unique_handle>;
    using transaction_ref    = basic_transaction<handle_ref>;

} // namespace ntw::io

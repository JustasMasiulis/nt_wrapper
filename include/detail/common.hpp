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
#define NOMINMAX
#include <type_traits>
#include <utility>
#include "../../deps/lazy_importer/include/lazy_importer.hpp"
#include "../../deps/phnt/phnt_windows.h"
#include "../../deps/phnt/phnt.h"

#ifdef NTW_NO_LAZY_IMPORTS
#define LI_NT(x) x
#endif

#ifdef NTW_ENABLE_DEBUG_TRACING
#include <cstdio>
#define NTW_NT_TRACE(name, status)                                    \
    std::printf(__FILE__ " line: %i %s %u %s\n",                      \
                __LINE__,                                             \
                name,                                                 \
                status,                                               \
                (NT_ERROR(status) ? " error\n"                        \
                                  : (NT_SUCCESS(status) ? "success\n" \
                                                        : " might not be an error\n")));

#define NTW_TRACE_OR_CRASH(name)                                 \
    {                                                            \
        std::printf(__FILE__ " line: %i %s \n", __LINE__, name); \
        __debugbreak();                                          \
    }

#define LI_NT(x)                                                       \
    [&](auto... args) {                                                \
        const auto status = reinterpret_cast<decltype(&x)>(            \
            ::li::detail::find_nt<::li::detail::hash(#x)>())(args...); \
        if(!NT_SUCCESS(status))                                        \
            NTW_NT_TRACE(#x, status);                                  \
        return status;                                                 \
    }


#else
#define NTW_NT_TRACE(...)
#define NTW_TRACE_OR_CRASH(name) __ud2();
#endif

#ifndef NTW_INLINE
#ifdef _MSC_VER
#define NTW_INLINE __forceinline
#else
#endif
#endif

#define NT_FN [[nodiscard]] NTW_INLINE NTSTATUS

#define NTW_QUERY_BUFFER_REQUIREMENT \
    class = std::enable_if_t<        \
        !std::is_invocable_v<Buffer, void*, std::size_t, unsigned long, unsigned long*>>

#define NTW_QUERY_CALLBACK_REQUIREMENT \
    class = std::enable_if_t<std::is_invocable_v<Callback, void*, std::size_t, Args...>>


#define NTW_CALLBACK_BREAK_IF_FALSE(callback, ...)                              \
    if constexpr(std::is_same<decltype(callback(__VA_ARGS__)), bool>::value) {  \
        if(!::ntw::detail::callback_invoker<decltype(callback(__VA_ARGS__))>{}( \
               callback, __VA_ARGS__))                                          \
            break;                                                              \
    }                                                                           \
    else                                                                        \
        callback(__VA_ARGS__);

#ifdef NTW_DEBUG_TRACE_ALL
#define ret_on_err(expr)                  \
    {                                     \
        const auto ret_err_expr = expr;   \
        NTW_NT_TRACE(#expr, ret_err_expr) \
        if(!NT_SUCCESS(ret_err_expr)) {   \
            return ret_err_expr;          \
        }                                 \
    }

#define cont_on_err(expr)                  \
    {                                      \
        const auto cont_err_expr = expr;   \
        NTW_NT_TRACE(#expr, cont_err_expr) \
        if(!NT_SUCCESS(cont_err_expr))     \
            continue;                      \
    }
#else
#define ret_on_err(expr)                      \
    {                                         \
        const auto ret_err_expr = expr;       \
        if(!NT_SUCCESS(ret_err_expr)) {       \
            NTW_NT_TRACE(#expr, ret_err_expr) \
            return ret_err_expr;              \
        }                                     \
    }

#define cont_on_err(expr)                      \
    {                                          \
        const auto cont_err_expr = expr;       \
        if(!NT_SUCCESS(cont_err_expr)) {       \
            NTW_NT_TRACE(#expr, cont_err_expr) \
            continue;                          \
        }                                      \
    }
#endif

namespace ntw { namespace detail {

    template<class T>
    struct callback_invoker {
        template<class Fn, class... Args>
        bool operator()(Fn fn, Args&&... args) const
            noexcept(noexcept(fn(std::forward<Args>(args)...)))
        {
            return fn(std::forward<Args>(args)...);
        }
    };

}} // namespace ntw::detail

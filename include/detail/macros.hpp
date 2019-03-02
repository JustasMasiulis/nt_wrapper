#pragma once

#define NTW_INLINE __forceinline
#define NTW_SYSCALL(fn) fn
#define NTW_IMPORT_CALL(fn) fn

#define NTW_RET_ON_ERR(expr)                         \
    {                                                \
        ::ntw::status _ntw_ret_on_err_status = expr; \
        if(!_ntw_ret_on_err_status.success())        \
            return_ntw_ret_on_err_status;            \
    }
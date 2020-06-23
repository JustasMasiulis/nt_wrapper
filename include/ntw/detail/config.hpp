#pragma once

#define NTW_INLINE __forceinline

#ifndef NTW_SYSCALL
#define NTW_SYSCALL(fn) fn
#endif

#define NTW_IMPORT_CALL(fn) fn

#ifndef JMLOG_UTILS_H
#define JMLOG_UTILS_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <string>

#include "jmlog/common.h"

namespace jmlog {

template <typename T>
static int ptr2int(const T * ptr)
{
    uintptr_t val = reinterpret_cast<uintptr_t>(ptr);
    return static_cast<int>(val);
}

const char * string_fmt(const char * p = nullptr)
{
    (void)p;
    return "[%s] ";
}

const wchar_t * string_fmt(const wchar_t * p = nullptr)
{
    (void)p;
    return L"[%s] ";
}

} // namespace jmlog

#endif // JMLOG_UTILS_H

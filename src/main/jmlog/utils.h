
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

char pathCharToUpper(char c)
{
    if (c >= 'A') {
        unsigned char uc = (unsigned char)c;
        uc &= (~(unsigned char)0x40);
        return (char)uc;
    }
    else {
        return c;
    }
}

std::size_t getSourceRootDirOffset(const char * source_filename, const std::size_t len_source,
                                   const char * relative_filename, const std::size_t len_relative)
{
    if (len_source == 0 || len_relative == 0) {
        return std::size_t(-1);
    }

    std::ptrdiff_t pos = -1;
    if (len_source >= len_relative) {
        const char * source = source_filename + len_source - 1;
        const char * relative = relative_filename + len_relative - 1;
        while (relative > relative_filename) {
            if (pathCharToUpper(*relative) == pathCharToUpper(*source)) {
                relative--;
                source--;
            }
            else if (*relative == '\\' || *relative == '/') {
                if (*source == '\\' || *source == '/') {
                    relative--;
                    source--;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }

        pos = source - source_filename;
    }
    else {
        for (std::ptrdiff_t i = std::ptrdiff_t(len_source - 1); i >= 0; i--) {
            char ch = source_filename[i];
            if (ch == '\\' || ch == '/') {
                pos = i;
                break;
            }
        }
        if (pos == -1) {
            pos = len_source;
        }
    }

    return std::size_t(pos);
}

} // namespace jmlog

#endif // JMLOG_UTILS_H

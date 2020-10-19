
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

/*********************************************************************************/

//
// C++ 11
// Msvc 2015 update 3, gcc 5.4, clang 3.8.0
//
// __FILE__ macro shows full path
// See: https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
//
template <typename T, std::size_t Size>
inline
constexpr std::size_t getSourceFileNameOffset(const T (& str)[Size], std::size_t i = Size - 1)
{
    return (str[i] == '/' || str[i] == '\\') ? (i + 1) : ((i > 0) ? getSourceFileNameOffset(str, i - 1) : 0);
}

template <typename T>
inline
constexpr std::size_t getSourceFileNameOffset(T (& str)[1])
{
    return 0;
}

/*********************************************************************************/

//
// C++ 11
//
// How to extract the source filename without path and suffix at compile time?
// https://stackoverflow.com/questions/31050113/how-to-extract-the-source-filename-without-path-and-suffix-at-compile-time
//
constexpr const char * str_end(const char * str) {
    return ((*str) ? str_end(str + 1) : str);
}

constexpr bool str_slant(const char * str) {
    return (*str == '/' || *str == '\\') ? true : ((*str) ? str_slant(str + 1) : false);
}

constexpr const char * reverse_slant(const char * str) {
    return (*str == '/' || *str == '\\') ? (str + 1) : reverse_slant(str - 1);
}

constexpr const char * getSourceFileName_cxx11_original(const char * str) {
    return str_slant(str) ? reverse_slant(str_end(str)) : str;
}

/*********************************************************************************/

//
// C++ 11
//
// How to extract the source filename without path and suffix at compile time?
// https://stackoverflow.com/questions/31050113/how-to-extract-the-source-filename-without-path-and-suffix-at-compile-time
//
constexpr bool is_null_terminator(char c) {
    return (c == '\0');
}

constexpr bool is_not_null_terminator(char c) {
    return (c != '\0');
}

constexpr bool is_path_separator(char c) {
    return (c == '/' || c == '\\');
}

constexpr const char * find_str_end(const char * str) {
    return (is_not_null_terminator(*str) ? find_str_end(str + 1) : str);
}

constexpr bool find_first_of_separator(const char * str) {
    return (is_path_separator(*str) ? true : (is_not_null_terminator(*str) ? find_first_of_separator(str + 1) : false));
}

constexpr const char * reverse_find_first_of_separator(const char * str) {
    return (is_path_separator(*str) ? (str + 1) : reverse_find_first_of_separator(str - 1));
}

constexpr const char * getSourceFileName_cxx11(const char * str) {
    return (find_first_of_separator(str) ? reverse_find_first_of_separator(find_str_end(str)) : str);
}

/*********************************************************************************/

namespace cxx14 {
namespace v1 {

//
// C++ 14
//
char pathCharToUpper(char c)
{
    if (c == '\\')
        return '/';
    else
        return ((c >= 'a' && c <= 'z') ? (c - 0x20) : c);
}

bool isPathSeparator(char c)
{
    return (c == '/' || c == '\\');
}

bool lastCharIsPathSeparator(const char * first, const char * last)
{
    ptrdiff_t size = last - first;
    if (size > 0) {
        const char * tail = last - 1;
        return isPathSeparator(*tail);
    }
    return false;
}

const char * getSourceFileName(const char * source_path,
                               const char * source_prefix,
                               bool include_prefix)
{
    const char * filename = source_path;
    const char * path = source_path;
    while (*path != '\0') {
        const char * target = source_prefix;
        // Find first char equal target
        do {
            if (pathCharToUpper(*path) != pathCharToUpper(*target))
                path++;
            else
                break;
        } while (*path != '\0');
        const char * start_pos = path;
        do {
            if (pathCharToUpper(*path) != pathCharToUpper(*target)) {
                if (*target != '\0') {
                    path++;
                    break;
                }
                else {
                    // Target reach the end of and path is not a null terminator.
                    if (target <= source_prefix) {
                        return source_path;
                    }
                    bool lastTargetIsSep = lastCharIsPathSeparator(source_prefix, target);
                    bool curPathIsSep = isPathSeparator(*path);
                    if ((lastTargetIsSep && !curPathIsSep) ||
                        (!lastTargetIsSep && curPathIsSep)) {
                        if (include_prefix) {
                            filename = path - (target - source_prefix);
                        }
                        else {
                            bool lastPathIsSep = lastCharIsPathSeparator(source_path, path);
                            if (lastTargetIsSep && lastPathIsSep)
                                filename = path - 1;
                            else
                                filename = path;
                        }
                    }
                    // In fact, start_pos = path - (target - source_prefix)
                    path = start_pos + 1;
                    break;
                }
            }
            else {
                if (*target != '\0') {
                    path++;
                    target++;
                }
                else {
                    // Target reach the end of and both path and target is null terminator.
                    if (target <= source_prefix) {
                        return source_path;
                    }
                    bool lastTargetIsSep = lastCharIsPathSeparator(source_prefix, target);
                    bool curPathIsSep = isPathSeparator(*path);
                    if ((lastTargetIsSep && !curPathIsSep) ||
                        (!lastTargetIsSep && curPathIsSep)) {
                        if (include_prefix) {
                            filename = path - (target - source_prefix);
                        }
                        else {
                            bool lastPathIsSep = lastCharIsPathSeparator(source_path, path);
                            if (lastTargetIsSep && lastPathIsSep)
                                filename = path - 1;
                            else
                                filename = path;
                        }
                    }
                    // In fact, start_pos = path - (target - source_prefix)
                    path = start_pos + 1;
                    break;
                }
            }
        } while (1);
    }
    return filename;
}

} // namespace v1
} // namespace cxx14

/*********************************************************************************/

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

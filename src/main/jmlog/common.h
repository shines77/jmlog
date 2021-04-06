
#ifndef JMLOG_COMMON_H
#define JMLOG_COMMON_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdio.h>
#include <stdint.h>

#include <cstdint>
#include <cstddef>

#ifndef __WCHAR
#define __WCHAR(x)          L ## x
#endif

#ifndef JMLOG_WCHAR
#define JMLOG_WCHAR(x)      __WCHAR(x)
#endif

#define JMLOG_ENABLE_CHECK_PRINTF_FORMAT    1

//
// __attribute__((format(printf, 1, 2))) for MSVC?
// See: https://stackoverflow.com/questions/2354784/attribute-formatprintf-1-2-for-msvc
//
// SAL Annotations
// See: https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/ms235402(v=vs.100)?redirectedfrom=MSDN
//

/* NOTE: use /analyze or _USE_ATTRIBUTES_FOR_SAL = 1 for checking */

#ifndef JMLOG_MSVC_FORMAT_STRING
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
# include <sal.h>
# if (_MSC_VER > 1400)
#  define JMLOG_MSVC_FORMAT_STRING    _Printf_format_string_
# else
#  define JMLOG_MSVC_FORMAT_STRING    __format_string
# endif
#else
# define JMLOG_MSVC_FORMAT_STRING
#endif // _MSC_VER
#endif // JMLOG_MSVC_FORMAT_STRING

#ifndef JMLOG_GCC_CHECK_PRINTF_FORMAT
#if (defined(__GUNC__) || defined(__clang__)) && defined(__attribute__)
  //
  // GCC C-FLAGS: -Wformat is enable
  // arche_type: printf, scanf.
  //
  // __attribute__((format(arche-type, string-index, first-to-check)))
  //
  // See: https://blog.csdn.net/huangjh2017/article/details/76944564
  //
  // void Trace(const char * fmt, ...) __attribute__((format(printf, 1, 2)));
  //
  // void Trace(const char * fmt, ...)
  // {
  //    va_list ap;
  //    va_start(ap, fmt);
  //    (void)printf(fmt, ap);
  //    va_end(ap);
  // }
  //
# define JMLOG_GCC_CHECK_PRINTF_FORMAT(fmt_string_index, first_to_check_args_index) \
         __attribute__((format(printf, (fmt_string_index), (first_to_check_args_index))));
#else
# define JMLOG_GCC_CHECK_PRINTF_FORMAT(fmt_string_index, first_to_check_args_index)
#endif
#endif // JMLOG_GCC_CHECK_PRINTF_FORMAT

#if JMLOG_ENABLE_CHECK_PRINTF_FORMAT
# ifndef JMLOG_CHECK_PRINTF_FORMAT
#  define JMLOG_CHECK_PRINTF_FORMAT(...)   (0 && ::snprintf(nullptr, 0, ##__VA_ARGS__))
#  if defined(__cplusplus)
    // in C++ we can allow macros that optionally take a format parameter, typical of macros that
    // translate an empty format into a newline or such.
    static inline int snprintf(char * buf, std::size_t count) {
        return 0;
    }
#  endif
# endif // JMLOG_CHECK_PRINTF_FORMAT
#endif // JMLOG_ENABLE_CHECK_PRINTF_FORMAT

#endif // JMLOG_COMMON_H

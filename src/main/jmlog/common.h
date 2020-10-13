
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

#define ENABLE_MSVC_PRINTF_CHECK    1

//
// __attribute__((format(printf, 1, 2))) for MSVC?
// See: https://stackoverflow.com/questions/2354784/attribute-formatprintf-1-2-for-msvc
//
// SAL Annotations
// See: https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/ms235402(v=vs.100)?redirectedfrom=MSDN
//

/* use /analyze or _USE_ATTRIBUTES_FOR_SAL = 1 for checking */

#ifndef MSVC_FORMAT_STRING
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
# include <sal.h>
# if (_MSC_VER > 1400)
#  define MSVC_FORMAT_STRING(p)     _Printf_format_string_ p
# else
#  define MSVC_FORMAT_STRING(p)     __format_string p
# endif
#else
# define MSVC_FORMAT_STRING(p)      p
#endif // _MSC_VER
#endif // MSVC_FORMAT_STRING

#if (defined(__GUNC__) || defined(__clang__)) && defined(__attribute__)
  //
  // GCC C-FLAGS: -Wformat is enable
  // arche_type: printf, scanf.
  //
# define JMLOG_FORMAT_ATTR(arche_type, fmt_index, args_index) \
        __attribute__((format(arche_type, (fmt_index), (args_index))));
#else
# define JMLOG_FORMAT_ATTR(arche_type, fmt_index, args_index)
#endif

#if ENABLE_MSVC_PRINTF_CHECK
# ifndef MSVC_PRINTF_CHECK
#  define MSVC_PRINTF_CHECK(...)    (0 && ::snprintf(nullptr, 0, ##__VA_ARGS__))
#  if defined(__cplusplus)
    // in C++ we can allow macros that optionally take a format parameter, typical of macros that
    // translate an empty format into a newline or such.
    static inline int snprintf(const char * fmt, std::size_t len) {
        return 0;
    }
#  endif
# else
#  define MSVC_PRINTF_CHECK(...)    ((void)(0))
# endif // MSVC_PRINTF_CHECK
#endif // ENABLE_MSVC_PRINTF_CHECK

#define jmLog_Write_Info(log, pattern, ...) \
    do { int tmp = MSVC_PRINTF_CHECK(__VA_ARGS__); log.info(pattern, __VA_ARGS__); } while (0)

#endif // JMLOG_COMMON_H

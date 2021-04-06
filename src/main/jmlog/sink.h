
#ifndef JMLOG_SINK_H
#define JMLOG_SINK_H

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

#include "jmlog/data_type.h"
#include "jmlog/log_level.h"
#include "jmlog/common.h"

namespace jmlog {

template <typename CharTy>
class BaseSink {
public:
    typedef CharTy                  char_type;
    typedef std::size_t             size_type;
    typedef BaseSink<CharTy>        this_type;
    typedef BasicPattern<CharTy>    pattern_type;

public:
    BaseSink() {}
    ~BaseSink() {}
};

template <typename CharTy>
class BasicConsoleSink : public BaseSink<CharTy> {
public:
    typedef BasicConsoleSink<CharTy>            this_type;
    typedef BaseSink<CharTy>                    base_type;
    typedef CharTy                              char_type;
    typedef typename base_type::size_type       size_type;
    typedef typename base_type::pattern_type    pattern_type;

public:
    BasicConsoleSink() {}
    ~BasicConsoleSink() {}

    int print(const char_type * fmt, va_list arg_list) {
        return ::vprintf(fmt, arg_list);
    }

    int print(const char_type * buf, size_type buf_size,
              const char_type * fmt, va_list arg_list) {
        return ::vsnprintf(buf, buf_size, fmt, arg_list);
    }

    //template <typename ...Args>
    //int print(const pattern_type & pattern, Args && ... args) {
    //    return ::printf(pattern.getFormat().c_str(), std::forward<Args>(args)...);
    //}

    int print(const pattern_type * pattern, JMLOG_MSVC_FORMAT_STRING const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        int result = ::vprintf(pattern->getFormat().c_str(), arg_list);
        va_end(arg_list);
        return result;
    }
};

} // namespace jmlog

#endif // JMLOG_SINK_H

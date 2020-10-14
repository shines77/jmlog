
#ifndef JMLOG_LOGGER_H
#define JMLOG_LOGGER_H

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
#include <unordered_map>

#include "jmlog/data_type.h"
#include "jmlog/log_level.h"
#include "jmlog/common.h"
#include "jmlog/global.h"
#include "jmlog/pattern.h"
#include "jmlog/sink.h"

namespace jmlog {

template <typename CharTy, typename Sinker = BasicConsoleSink<CharTy> >
class BasicLogger {
public:
    typedef CharTy                          char_type;
    typedef Sinker                          sink_type;
    typedef std::basic_string<char_type>    string_type;
    typedef BasicPattern<CharTy>            pattern_type;
    typedef BasicLogger<CharTy, Sinker>     this_type;

    typedef std::unordered_map<string_type, pattern_type>
                                            pattern_map_t;

protected:
    Level           level_;
    string_type     filename_;
    sink_type       sink_;
    pattern_map_t   pattern_map_;

public:
    BasicLogger(Level level = Level::Fatal) : level_(level) {}
    ~BasicLogger() {}

    Level getLevel() const {
        return this->level_;
    }

    void setLevel(Level level) {
        this->level_ = level;
    }

    string_type & getLogFile() {
        return this->filename_;
    }

    const string_type & getLogFile() const {
        return this->filename_;
    }

    void setLogFile(const string_type & filename) {
        this->filename_ = filename;
    }

    const pattern_type & registerPattern(const char_type * filename, std::size_t line_num, 
                                         MSVC_FORMAT_STRING(const char_type * fmt), ...)
                                         JMLOG_FORMAT_ATTR(printf, 3, 4) {
        typedef typename pattern_map_t::const_iterator  const_iterator;
        const_iterator iter = this->pattern_map_.find(fmt);
        if (iter != this->pattern_map_.end()) {
            return iter->second;
        }
        else {
            pattern_type pattern(fmt, line_num, filename);
            auto result = this->pattern_map_.emplace(fmt, pattern);
            if (result.second == true) {
                const_iterator value = result.first;
                return value->second;
            }
            else {
                throw std::runtime_error("BasicLogger<T>::registerPattern() failed.");
            }
        }
    }

    void log(Level level, MSVC_FORMAT_STRING(const char_type * fmt), ...) 
         JMLOG_FORMAT_ATTR(printf, 2, 3) {
        if (this->getLevel() >= level) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(level, fmt, arg_list);
            va_end(arg_list);
        }
    }

    void trace(MSVC_FORMAT_STRING(const char_type * fmt), ...) {
        if (this->getLevel() >= Level::Trace) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(Level::Trace, fmt, arg_list);
            va_end(arg_list);
        }
    }

    void debug(MSVC_FORMAT_STRING(const char_type * fmt), ...) {
        if (this->getLevel() >= Level::Debug) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(Level::Debug, fmt, arg_list);
            va_end(arg_list);
        }
    }

    void warn(MSVC_FORMAT_STRING(const char_type * fmt), ...)
        JMLOG_FORMAT_ATTR(printf, 1, 2) {
        if (this->getLevel() >= Level::Warn) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(Level::Warn, fmt, arg_list);
            va_end(arg_list);
        }
    }

    void info(MSVC_FORMAT_STRING(const char_type * fmt), ...)
        JMLOG_FORMAT_ATTR(printf, 1, 2) {
        if (this->getLevel() >= Level::Info) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(Level::Info, fmt, arg_list);
            va_end(arg_list);
        }
    }

    template <typename ...Args>
    void info(const pattern_type & pattern,
              MSVC_FORMAT_STRING(const char_type * fmt),
              Args && ... args) 
        JMLOG_FORMAT_ATTR(printf, 2, 3) {
        if (this->getLevel() >= Level::Info) {
            (void)fmt;
            this->print(pattern, fmt, std::forward<Args>(args)...);
        }
    }

    void error(MSVC_FORMAT_STRING(const char_type * fmt), ...)
        JMLOG_FORMAT_ATTR(printf, 1, 2) {
        if (this->getLevel() >= Level::Error) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(Level::Error, fmt, arg_list);
            va_end(arg_list);
        }
    }

    void fatal(MSVC_FORMAT_STRING(const char_type * fmt), ...)
        JMLOG_FORMAT_ATTR(printf, 1, 2) {
        if (this->getLevel() >= Level::Fatal) {
            va_list arg_list;
            va_start(arg_list, fmt);
            this->log(Level::Fatal, fmt, arg_list);
            va_end(arg_list);
        }
    }

protected:
    void log(Level level, const char_type * fmt, va_list arg_list) {
        this->print_args(string_fmt(fmt), levelToStr(level, fmt));
        this->print(fmt, arg_list);
    }

    int print_args(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        int result = this->sink_.print(fmt, arg_list);
        va_end(arg_list);
        return result;
    }

    int print(const char_type * fmt, va_list arg_list) {
        return this->sink_.print(fmt, arg_list);
    }

    template <typename ...Args>
    int print(const pattern_type & pattern, const char_type * fmt, Args && ... args) {
        return this->sink_.print(&pattern, fmt, std::forward<Args>(args)...);
    }
};

typedef BasicLogger<char>       Logger;
typedef BasicLogger<wchar_t>    WLogger;

} // namespace jmlog

#endif // JMLOG_LOGGER_H


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

namespace jmlog {

static std::string jmlog_log_filename;

void setGlobalLogFile(const std::string & filename)
{
    jmlog_log_filename = filename;
}

enum class Level : int {
    None = 0,
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Last
};
template <typename CharTy>
class BaseSink {
public:
    typedef CharTy              char_type;
    typedef std::size_t         size_type;
    typedef BaseSink<CharTy>    this_type;

public:
    BaseSink() {}
    ~BaseSink() {}
};

template <typename CharTy>
class BasicConsoleSink : public BaseSink<CharTy> {
public:
    typedef BasicConsoleSink<CharTy>        this_type;
    typedef BaseSink<CharTy>                base_type;
    typedef CharTy                          char_type;
    typedef typename base_type::size_type   size_type;

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
};

template <typename CharTy, typename Sinker = BasicConsoleSink<CharTy> >
class BasicLogger {
public:
    typedef CharTy                      char_type;
    typedef Sinker                      sink_type;
    typedef BasicLogger<CharTy, Sinker> this_type;

protected:
    Level level_;
    std::string filename_;
    sink_type sink_;

public:
    BasicLogger(Level level = Level::Fatal) : level_(level) {}
    ~BasicLogger() {}

    Level getLevel() const {
        return this->level_;
    }

    void setLevel(Level level) {
        this->level_ = level;
    }

    void setLogFile(const std::string & filename) {
        this->filename_ = filename;
    }

    void log(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void debugTrace(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void trace(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void debug(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void warn(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void info(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void error(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    void fatal(const char_type * fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

protected:
    int print(const char_type * fmt, va_list arg_list) {
        return this->sink_.print(fmt, arg_list);
    }
};

typedef BasicLogger<char>       Logger;
typedef BasicLogger<wchar_t>    WLogger;

} // namespace jmlog

#endif // JMLOG_LOGGER_H

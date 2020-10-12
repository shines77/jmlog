
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
#include <functional>

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
//# pragma warning(push)
# include <sal.h>
//# pragma warning(pop)
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

namespace jm {

enum _DataType {
    CHAR,
    UCHAR,
    WCHAR,
    STR,
    WSTR,
    SHORT,
    USHORT,
    INT,
    UINT,
    LONG,
    ULONG,
    LONGLONG,
    ULONGLONG,
    FLOAT,
    DOUBLE,
    VOID_PTR,
    LAST_TYPE = VOID_PTR
};

enum DataType {
    i8,
    u8,
    i16,
    u16,
    i32,
    u32,
    l32,
    ul32,
    i64,
    u64,
    Bool,
    Char,
    Byte,
    WChar,
    String,
    WString,
    Float,
    Double,
    VoidPtr,
    LastType = VoidPtr
};

} // namespace jm

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
class BasicPattern {
private:
    std::string fmt_;

public:
    BasicPattern(const std::string & fmt) : fmt_(fmt) {}
    ~BasicPattern() {}

    const std::string & getFormat() const {
        return this->fmt_;
    }

    void setFormat(const std::string & fmt) {
        this->fmt_ = fmt;
    }
};

typedef BasicPattern<char>       Pattern;
typedef BasicPattern<wchar_t>    WPattern;

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

    int print(const pattern_type * pattern, MSVC_FORMAT_STRING(const char_type * fmt), ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        int result = ::vprintf(pattern->getFormat().c_str(), arg_list);
        va_end(arg_list);
        return result;
    }
};

template <typename CharTy, typename Sinker = BasicConsoleSink<CharTy> >
class BasicLogger {
public:
    typedef CharTy                      char_type;
    typedef Sinker                      sink_type;
    typedef BasicPattern<CharTy>        pattern_type;
    typedef BasicLogger<CharTy, Sinker> this_type;

    typedef std::unordered_map<std::string, pattern_type>
                                        pattern_map_t;

protected:
    Level           level_;
    std::string     filename_;
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

    std::string & getLogFile() {
        return this->filename_;
    }

    const std::string & getLogFile() const {
        return this->filename_;
    }

    void setLogFile(const std::string & filename) {
        this->filename_ = filename;
    }

    const pattern_type & createPattern(const char_type * fmt, ...) {
        pattern_type pattern(fmt);
        auto result = pattern_map_.emplace(fmt, pattern);
        typename pattern_map_t::const_iterator iter = result.first;
        return iter->second;        
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

    void info(MSVC_FORMAT_STRING(const char_type * fmt), ...)
        JMLOG_FORMAT_ATTR(printf, 1, 2) {
        va_list arg_list;
        va_start(arg_list, fmt);
        this->print(fmt, arg_list);
        va_end(arg_list);
    }

    template <typename ...Args>
    void info(const pattern_type & pattern,
              MSVC_FORMAT_STRING(const char_type * fmt),
              Args && ... args) 
        JMLOG_FORMAT_ATTR(printf, 2, 3) {
        (void)fmt;
        this->print(pattern, fmt, std::forward<Args>(args)...);
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

    template <typename ...Args>
    int print(const pattern_type & pattern, const char_type * fmt, Args && ... args) {
        return this->sink_.print(&pattern, fmt, std::forward<Args>(args)...);
    }
};

typedef BasicLogger<char>       Logger;
typedef BasicLogger<wchar_t>    WLogger;

} // namespace jmlog

#endif // JMLOG_LOGGER_H

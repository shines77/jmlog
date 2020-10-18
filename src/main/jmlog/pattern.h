
#ifndef JMLOG_PATTERN_H
#define JMLOG_PATTERN_H

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

#include "jmlog/log_level.h"
#include "jmlog/common.h"

#define jmlog_write_log(logger, level, pattern, fmt, ...) \
    do { \
        int tmp = MSVC_PRINTF_CHECK(fmt, ##__VA_ARGS__);    \
        logger.log(level, fmt, ##__VA_ARGS__);              \
    } while (0)

#define jmlog_info(log, fmt, ...) \
    do { \
        int tmp = MSVC_PRINTF_CHECK(fmt, ##__VA_ARGS__);  \
        log.info(fmt, ##__VA_ARGS__);                     \
    } while (0)

#define jmRegisterPattern(log, pattern, fmt, ...) \
    do { \
        int tmp = MSVC_PRINTF_CHECK(fmt, ##__VA_ARGS__);    \
        int pattern_id = jmlog::ptr2int(fmt);               \
    } while (0);                                            \
                                                            \
    const jmlog::Pattern & pattern = log.registerPattern(   \
    __FILE__, std::size_t(__LINE__), fmt, ##__VA_ARGS__)

namespace jmlog {

template <typename CharTy>
class BasicPattern {
public:
    typedef CharTy                          char_type;
    typedef std::basic_string<char_type>    string_type;
    typedef BasicPattern<CharTy>            this_type;

private:
    const char_type *   fmt_;
    std::size_t         line_num_;
    string_type         format_;
    string_type         filename_;

public:
    explicit BasicPattern(const char_type * fmt) : fmt_(fmt), line_num_(-1), format_(fmt) {
    }
    BasicPattern(const char_type * fmt,
                 std::size_t line_num,
                 const string_type & filename)
        : fmt_(fmt), line_num_(line_num), format_(fmt), filename_(filename) {
    }
    ~BasicPattern() {}

    string_type & getFmt() {
        return this->fmt_;
    }

    const string_type & getFmt() const {
        return this->fmt_;
    }

    string_type & getFormat() {
        return this->format_;
    }

    const string_type & getFormat() const {
        return this->format_;
    }

    void setFormat(const char_type * fmt) {
        this->fmt_ = fmt;
        this->format_ = fmt;
    }

    string_type & getSourceFile() {
        return this->filename_;
    }

    const string_type & getSourceFile() const {
        return this->filename_;
    }

    void setSourceFile(const string_type & filename) {
        this->filename_ = filename;
    }

    std::size_t getLineNum() const {
        return this->line_num_;
    }

    void setLineNum(std::size_t line_no) {
        this->line_num_ = line_no;
    }
};

typedef BasicPattern<char>       Pattern;
typedef BasicPattern<wchar_t>    WPattern;

} // namespace jmlog

#endif // JMLOG_PATTERN_H

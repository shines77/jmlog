
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

#define jmLog_Write_Info(log, pattern, fmt, ...) \
    do { \
        int tmp = MSVC_PRINTF_CHECK(fmt, __VA_ARGS__);  \
        log.info(pattern, fmt, __VA_ARGS__);            \
    } while (0)

#define jmRegisterPattern(log, fmt, ...) \
    do { \
        int tmp = MSVC_PRINTF_CHECK(fmt, __VA_ARGS__);          \
        int pattern_id = jmlog::ptr2int(fmt);                   \
        const Pattern & __pattern = log.registerPattern(        \
            __FILE__, std::size_t(__LINE__), fmt, __VA_ARGS__); \
        log.info(__pattern, fmt, __VA_ARGS__);                  \
    } while (0)

#define jmRegisterPattern2(log, fmt, ...) \
    do { \
        int tmp = MSVC_PRINTF_CHECK(fmt, __VA_ARGS__);          \
        int pattern_id = jmlog::ptr2int(fmt);                   \
        const Pattern & __pattern = log.registerPattern(        \
            __FILE__, std::size_t(__LINE__), fmt, __VA_ARGS__); \
        log.info(__pattern, fmt, __VA_ARGS__);                  \
    } while (0)

namespace jmlog {

template <typename CharTy>
class BasicPattern {
public:
    typedef CharTy                          char_type;
    typedef std::basic_string<char_type>    string_type;
    typedef BasicPattern<CharTy>            this_type;

private:
    const char_type *   fmt_;
    std::size_t         line_no_;
    string_type         format_;
    string_type         filename_;

public:
    BasicPattern(const char_type * fmt) : fmt_(fmt), line_no_(-1), format_(fmt) {
    }
    BasicPattern(const char_type * fmt,
                 std::size_t line_no,
                 const string_type & filename)
        : fmt_(fmt), line_no_(line_no), format_(fmt), filename_(filename) {
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

    std::size_t & getLineNo() const {
        return this->line_no_;
    }

    void setLineNo(std::size_t line_no) {
        this->line_no_ = line_no;
    }
};

typedef BasicPattern<char>       Pattern;
typedef BasicPattern<wchar_t>    WPattern;

} // namespace jmlog

#endif // JMLOG_PATTERN_H

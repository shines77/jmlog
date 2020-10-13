
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

namespace jmlog {

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

} // namespace jmlog

#endif // JMLOG_PATTERN_H

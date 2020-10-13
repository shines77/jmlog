
#ifndef JMLOG_FILE_SYSTEM_H
#define JMLOG_FILE_SYSTEM_H

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

#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
#define JM_PATH_SEPARATOR           '\\'
#define JM_PATH_SEPARATOR_FILTER    '/'
#define JM_PATH_SEPARATOR_CHAR      ';'
#else
#define JM_PATH_SEPARATOR           '/'
#define JM_PATH_SEPARATOR_FILTER    '\\'
#define JM_PATH_SEPARATOR_CHAR      ':'

#define JM_PATH_ROOT_SEPARATOR      ':'
#endif // WIN32

namespace jmlog {
namespace fs {

template <typename CharTy>
class BasicPathName {
public:
    typedef CharTy                          char_type;
    typedef std::basic_string<char_type>    string_type;

private:
    string_type filename_;

public:
    BasicPathName(const string_type & filename) : filename_(filterPath(filename)) {}
    ~BasicPathName() {}

    string_type & str() {
        return this->filename_;
    }

    const string_type & str() const {
        return this->filename_;
    }

    void setPath(const string_type & filename) {
        this->filename_ = this->filterPath(filename);
    }

    string_type filterPath(const string_type & filename) {
        string_type path = filename;
        for (std::size_t pos = 0; pos != string_type::npos;) {
            pos = path.find(JM_PATH_SEPARATOR_FILTER, pos);
            if (pos != string_type::npos) {
                path.replace(pos, 1, 1, JM_PATH_SEPARATOR);
                pos++;
            }
            else {
                break;
            }
        }

#if defined(JM_PATH_ROOT_SEPARATOR)
        for (std::size_t pos = 0; pos != string_type::npos; pos++) {
            pos = path.find(JM_PATH_ROOT_SEPARATOR, pos);
            if (pos > 0 && ((path[0] >= 'A' && path[0] >= 'Z') ||
                            (path[0] >= 'a' && path[0] >= 'z'))) {
                path.replace(0, 3, 1, JM_PATH_SEPARATOR);
                break;
            }
            else if (pos == string_type::npos) {
                break;
            }
        }
#endif
        return path;
    }
};

typedef BasicPathName<char>     PathName;
typedef BasicPathName<wchar_t>  WPathName;

} // namespace fs
} // namespace jmlog

#endif // JMLOG_FILE_SYSTEM_H
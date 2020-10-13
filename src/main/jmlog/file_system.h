
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
#define JM_PATH_DELIMITER           ';'
#else
#define JM_PATH_SEPARATOR           '/'
#define JM_PATH_SEPARATOR_FILTER    '\\'
#define JM_PATH_DELIMITER           ':'

#define JM_PATH_ROOT_SEPARATOR      ':'
#endif // WIN32

//
// Node.js - filesystem - path
// See: http://nodejs.cn/api/path.html
//

namespace jmlog {
namespace fs {

static bool is_windows() {
#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
    return true;
#else
    return false;
#endif
}

template <typename CharTy>
class BasicPath {
public:
    typedef CharTy                          char_type;
    typedef std::basic_string<char_type>    string_type;
    typedef BasicPath<char_type>            this_type;

    static const char_type kSeparator = char_type(JM_PATH_SEPARATOR);
    static const char_type kSeparatorFilter = char_type(JM_PATH_SEPARATOR_FILTER);
    static const char_type kDelimiter = char_type(JM_PATH_DELIMITER);

#if defined(JM_PATH_ROOT_SEPARATOR)
    static const char_type kRootSeparator = char_type(JM_PATH_ROOT_SEPARATOR);
#endif

private:
    string_type filename_;

public:
    BasicPath(const string_type & filename)
        : filename_(this_type::filterPath(filename)) {
    }
    ~BasicPath() {}

    string_type & filename() {
        return this->filename_;
    }

    const string_type & filename() const {
        return this->filename_;
    }

    void setPath(const string_type & filename) {
        this->filename_ = this->filterPath(filename);
    }

    static
    string_type filterPath(const string_type & filename) {
        string_type path = filename;
        for (std::size_t pos = 0; pos != string_type::npos;) {
            pos = path.find(kSeparatorFilter, pos);
            if (pos != string_type::npos) {
                path.replace(pos, 1, 1, kSeparator);
                pos++;
            }
            else {
                break;
            }
        }

#if defined(JM_PATH_ROOT_SEPARATOR)
        for (std::size_t pos = 0; pos != string_type::npos; pos++) {
            pos = path.find(kRootSeparator, pos);
            if (pos > 0 && ((path[0] >= 'A' && path[0] >= 'Z') ||
                            (path[0] >= 'a' && path[0] >= 'z'))) {
                path.replace(0, 3, 1, kSeparator);
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

typedef BasicPath<char>     Path;
typedef BasicPath<wchar_t>  WPath;

} // namespace fs
} // namespace jmlog

#endif // JMLOG_FILE_SYSTEM_H


#ifndef JMLOG_CONFIG_H
#define JMLOG_CONFIG_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>     // For ::strerror()
#include <errno.h>

#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <fstream>
#include <string>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

#include "jmlog/common.h"
#include "jmlog/file_system.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#include <Windows.h>
#undef  NOMINMAX
#else
#include <Windows.h>
#endif // NOMINMAX
#endif // WIN32_LEAN_AND_MEAN

#endif // WIN32

//
// About std::ifstream
// See: https://blog.csdn.net/sinat_36219858/article/details/80369255
//

#define JMLOG_DEFAULT_ROOT_DIR      "./LogRoot"

namespace jmlog {

std::basic_string<char> getDefaultRootDir(std::basic_string<char> * p = nullptr)
{
    (void)p;
    return std::basic_string<char>(JMLOG_DEFAULT_ROOT_DIR);
}

std::basic_string<wchar_t> getDefaultRootDir(std::basic_string<wchar_t> * p = nullptr)
{
    (void)p;
    return std::basic_string<wchar_t>(JMLOG_WCHAR(JMLOG_DEFAULT_ROOT_DIR));
}

template <typename CharTy>
class BasicConfigFile {
public:
    typedef CharTy                          char_type;
    typedef std::basic_string<char_type>    string_type;
    typedef fs::BasicPathName<char_type>    path_name_t;

private:
    string_type filename_;
    string_type root_dir_;
    string_type content_;

public:
    BasicConfigFile() {
        this->init();
    }
    explicit BasicConfigFile(const path_name_t & filename) : filename_(filename.str()) {
        this->loadConfig(this->filename_);
    }

    ~BasicConfigFile() {}

    void init() {
        string_type tmp;
        path_name_t dir(getDefaultRootDir(&tmp));
        this->root_dir_ = dir.str();
    }

    int setLogRootDirectory(const path_name_t & root_dir) {
        this->root_dir_ = root_dir.str();
        return 0;
    }

    int loadConfig(const path_name_t & pathname) {
        int result = 0;
        static const std::size_t kBufSize = 8192;
        std::ifstream ifs;
        const string_type & filename = pathname.str();
        try {
            ifs.open(filename, std::ios::in | std::ios::binary);
            if (ifs.is_open()) {
                ifs.seekg(0, std::ios::end);
                std::streampos file_size = ifs.tellg();
                if (file_size > 0) {
                    ifs.seekg(0, std::ios::beg);
                    this->content_.resize(std::size_t(file_size));
                    char * data = (char *)this->content_.data();
                    char buf[kBufSize];
                    while (ifs.good() && (file_size > 0)) {
                        ifs.read(buf, kBufSize);
                        std::streamsize read_bytes = ifs.gcount();
                        if (read_bytes > 0) {
                            std::memcpy(data, buf, read_bytes);
                            data += read_bytes;
                            file_size -= read_bytes;
                        }
                    }
                }
                ifs.close();
            }
            else {
#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
                result = (int)::GetLastError();
#else
                result = errno;
#endif
                ::printf("<<Error>>:\n\n"
                         "File: %s, Line: %u\n"
                         "errno: %d\n"
                         "Reason: %s\n\n"
                         "ConfigFile::loadConfig(filename):\n"
                         "filename = %s\n"
                         "Info: %s\n\n",
                         __FILE__, uint32_t(__LINE__),
                         result, ::strerror(result),
                         filename.c_str(),
                         "Read config failed.");
            }
        }
        catch (const std::exception & ex) {
#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
            result = (int)::GetLastError();
#else
            result = errno;
#endif
            ::printf("<<Exception>>:\n\n"
                     "File: %s, Line: %u\n"
                     "errno: %d\n"
                     "Reason: %s\n\n"
                     "ConfigFile::loadConfig(filename):\n"
                     "filename = %s\n"
                     "Info: %s\n\n",
                     __FILE__, uint32_t(__LINE__),
                     result, ::strerror(result),
                     filename.c_str(),
                     ex.what());
        }

        return result;
    }
};

typedef BasicConfigFile<char>       ConfigFile;
typedef BasicConfigFile<wchar_t>    WConfigFile;

} // namespace jmlog

#endif // JMLOG_CONFIG_H

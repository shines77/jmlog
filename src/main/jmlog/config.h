
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
#include <string.h>     // For strerror()
#include <errno.h>

#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <unordered_map>

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

namespace jmlog {

class ConfigFile {
private:
    std::string filename_;
    std::string root_dir_;
    std::string content_;

public:
    ConfigFile() {}
    explicit ConfigFile(const std::string & filename) : filename_(filename) {
        loadConfig(filename_);
    }

    ~ConfigFile() {}

    int loadConfig(const std::string & filename) {
        int result = 0;
        static const std::size_t kBufSize = 8192;
        std::ifstream ifs;
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
                        }
                        data += read_bytes;
                        file_size -= read_bytes;
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
                         "Reason: %s\n"
                         "ConfigFile::loadConfig(filename):\n"
                         "%s\n\n",
                         __FILE__, uint32_t(__LINE__),
                         result, ::strerror(result),
                         "Info: Read config failed.");
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
                     "Reason: %s\n"
                     "ConfigFile::loadConfig(filename):\n"
                     "Info: %s\n\n",
                     __FILE__, uint32_t(__LINE__),
                     result, ::strerror(result),
                     ex.what());
        }

        return result;
    }
};

} // namespace jmlog

#endif // JMLOG_CONFIG_H

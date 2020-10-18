
#ifndef JMLOG_GLOBAL_H
#define JMLOG_GLOBAL_H

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

#include "jmlog/common.h"
#include "jmlog/utils.h"
#include "jmlog/config.h"

#define jmGetSourceRootDirOffset(source_filename, relative_filename) \
    jmlog::getSourceRootDirOffset(source_filename, sizeof(source_filename), \
                                  relative_filename, sizeof(relative_filename))

static int32_t jm_i32 = 32;
static int64_t jm_i64 = 64;
static uint32_t jm_u32 = 32;
static uint64_t jm_u64 = 64;
static const char * jm_string = "jm_string";
static const wchar_t * jm_wstring = L"jm_wstring";

namespace jmlog {

static ConfigFile  jmlog_config;
static WConfigFile jmlog_wconfig;

static std::size_t kJmSourceRootDirOffset = -1;

template <typename CharTy>
static
BasicConfigFile<CharTy> & getGlobalConfig() {
    return jmlog_config;
}

static
BasicConfigFile<wchar_t> & getGlobalConfig() {
    return jmlog_wconfig;
}

template <typename CharTy>
static
bool init(const std::basic_string<CharTy> & conf_file,
          const std::basic_string<CharTy> & log_file)
{
    int loadStatus = getGlobalConfig<CharTy>().loadConfig(conf_file);
    return (loadStatus == 0);
}

template <typename CharTy>
static
bool init(const BasicConfigFile<CharTy> & config)
{
    BasicConfigFile<CharTy> & global_config = getGlobalConfig<CharTy>();
    global_config.copyConfig(config);
    kJmSourceRootDirOffset = jmGetSourceRootDirOffset(__FILE__, "/src/main/jmlog/global.h");
    return true;
}

void finalize()
{
    // unload jmlog resources
}

} // namespace jmlog

#endif // JMLOG_GLOBAL_H

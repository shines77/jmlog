
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
#include "jmlog/config.h"

namespace jmlog {

static ConfigFile  jmlog_config;
static WConfigFile jmlog_wconfig;

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
    getGlobalConfig<CharTy>() = config;
    return true;
}

void finalize()
{
    // unload jmlog resources
}

} // namespace jmlog

#endif // JMLOG_GLOBAL_H

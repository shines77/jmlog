
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

static std::string jmlog_log_filename;
static ConfigFile  jmlog_config;

void setGlobalLogFile(const std::string & filename)
{
    jmlog_log_filename = filename;
}

bool init(const std::string & conf_file, const std::string & log_file)
{
    int loadStatus = jmlog_config.loadConfig(conf_file);
    setGlobalLogFile(log_file);
    return true;
}

void finalize()
{
    // unload jmlog resources
}

} // namespace jmlog

#endif // JMLOG_GLOBAL_H

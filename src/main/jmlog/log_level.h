
#ifndef JMLOG_LOGLEVEL_H
#define JMLOG_LOGLEVEL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace jmlog {

enum Level : uint32_t {
    None = 0,
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Last
};

static const char * jmlog_level_str[] = {
    "None",
    "Trace",
    "Debug",
    "Info",
    "Warn",
    "Error",
    "Fatal",
    "Last"
};

static const wchar_t * jmlog_level_wstr[] = {
    L"None",
    L"Trace",
    L"Debug",
    L"Info",
    L"Warn",
    L"Error",
    L"Fatal",
    L"Last"
};

const char * levelToStr(Level level, const char * p = nullptr)
{
    (void)p;
    return jmlog_level_str[level];
}

const wchar_t * levelToStr(Level level, const wchar_t * p = nullptr)
{
    (void)p;
    return jmlog_level_wstr[level];
}

} // namespace jmlog

#endif // JMLOG_LOGLEVEL_H

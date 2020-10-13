
#ifndef JMLOG_LOGLEVEL_H
#define JMLOG_LOGLEVEL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace jmlog {

enum class Level : int {
    None = 0,
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Last
};

} // namespace jmlog

#endif // JMLOG_LOGLEVEL_H

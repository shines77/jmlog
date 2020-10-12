
//#ifndef _USE_ATTRIBUTES_FOR_SAL
//#define _USE_ATTRIBUTES_FOR_SAL     1
//#endif
//
//#ifndef _MSC_EXTENSIONS
//#define _MSC_EXTENSIONS
//#endif

#pragma warning(disable: 4467)

#include <jmlog/jmlog.h>

using namespace jmlog;

#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
#define BENCHMARK_LOG_FILE  ".\\benchmark-log.dat"
#else
#define BENCHMARK_LOG_FILE  "./benchmark-log.dat"
#endif

int main(int argc, char * argv[])
{
    jmlog::setGlobalLogFile(BENCHMARK_LOG_FILE);
    jmlog::Logger log(jmlog::Level::Info);
    log.setLogFile("test.log");
    log.info("value = %d, %d\n\n", 1121, 23213);
    const Pattern & pattern = log.createPattern("value = %d, %s\n\n", jm::i32, jm::String);
    log.info(pattern, "value = %d, %d\n\n", 1726187, "s");
    jmLog_Write_Info(log, pattern, "value = %d, %d\n\n", 1726187, 23123);
    return 0;
}

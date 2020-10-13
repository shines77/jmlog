
//#ifndef _USE_ATTRIBUTES_FOR_SAL
//#define _USE_ATTRIBUTES_FOR_SAL     1
//#endif

#define _CRT_SECURE_NO_WARNINGS

#if defined(_MSC_VER)
#pragma warning(disable: 4467)
#endif

#include <jmlog/jmlog.h>

using namespace jmlog;

#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
#define BENCHMARK_LOG_FILE  ".\\benchmark-log.dat"
#define BENCHMARK_CONF_FILE  ".\\benchmark-log.conf"
#else
#define BENCHMARK_LOG_FILE  "./benchmark-log.dat"
#define BENCHMARK_CONF_FILE  "./benchmark-log.conf"
#endif

int main(int argc, char * argv[])
{
    jmlog::init(BENCHMARK_CONF_FILE, BENCHMARK_LOG_FILE);

    //jmlog::setGlobalLogFile(BENCHMARK_LOG_FILE);
    jmlog::Logger log(jmlog::Level::Info);
    log.setLogFile("test.log");
    log.info("value = %d, %d\n\n", 1121, 23213);
    const Pattern & pattern = log.createPattern("value = %d, %s\n\n", jm::i32, jm::String);
    log.info(pattern, "value = %d, %d\n\n", 1726187, "str");
    jmLog_Write_Info(log, pattern, "value = %d, %d\n\n", 1726187, 232);

    jmlog::finalize();
    return 0;
}

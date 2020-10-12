
#include <jmlog/jmlog.h>

#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
#define BENCHMARK_LOG_FILE  ".\\benchmark-log.dat"
#else
#define BENCHMARK_LOG_FILE  "./benchmark-log.dat"
#endif

int main(int argn, char argv[])
{
    jmlog::setGlobalLogFile(BENCHMARK_LOG_FILE);
    jmlog::Logger log(jmlog::Level::Info);
    log.setLogFile("test.log");
    log.info("11211", 1121);
    return 0;
}

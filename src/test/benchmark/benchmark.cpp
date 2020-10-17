
//#ifndef _USE_ATTRIBUTES_FOR_SAL
//#define _USE_ATTRIBUTES_FOR_SAL     1
//#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4467)
#endif

#include <jmlog/jmlog.h>

using namespace jmlog;

#if defined(_MSC_VER)
#ifdef NDEBUG
#pragma comment(lib, "jmlog-Release.lib")
#else
#pragma comment(lib, "jmlog-Debug.lib")
#endif
#endif

#define BENCHMARK_CONF_FILE fs::Path("./benchmark-log.conf")
#define BENCHMARK_LOG_FILE  fs::Path("./benchmark-log.dat")
#define BENCHMARK_LOG_DIR   fs::Path("./benchmark-log")

int setting_conf_file(ConfigFile & config)
{
    config.loadConfig(BENCHMARK_CONF_FILE);
    config.setLogRootDirectory(BENCHMARK_LOG_DIR);
    return 1;
}

int main(int argc, char * argv[])
{
    ConfigFile config;
    setting_conf_file(config);

    jmlog::init(config);

    jmlog::Logger log(jmlog::Level::Info);
    log.setLogFile("test.log");
    log.info("value = %d, %d\n\n", 1121, 23213);
    const Pattern & pattern = log.registerPattern(__FILE__, __LINE__,
                                  "value = %d, %s\n\n", jm_i32, jm_string);
    log.info(pattern, "value = %d, %s\n\n", 1726187, "str");
    jmlog_info(log, "value = %d, %s\n\n", 1726187, "232");

    jmlog::finalize();
    return 0;
}

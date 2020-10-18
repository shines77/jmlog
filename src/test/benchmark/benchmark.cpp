
//#ifndef _USE_ATTRIBUTES_FOR_SAL
//#define _USE_ATTRIBUTES_FOR_SAL     1
//#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4467)
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <jmlog/jmlog.h>
#include <jmlog/rdtsc.h>

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

void test_cpu_fences()
{
    size_t * x = new size_t;
    static const size_t loops = 100000000; // 1 billion
    uint64_t start, elapsed;

    start = rdtsc();
    for (size_t i = 0; i < loops; i++) {
        *x = 0;
        _mm_lfence();
    }
    elapsed = rdtsc() - start;

    std::cout << "_mm_lfence       : " << elapsed << std::endl
              << "average          : " << double(elapsed) / double(loops) << " cycles " << std::endl;

    std::cout << std::endl;

    start = rdtsc();
    for (size_t i = 0; i < loops; i++) {
        *x = 0;
        _mm_sfence();
    }
    elapsed = rdtsc() - start;

    std::cout << "_mm_sfence       : " << elapsed << std::endl
              << "average          : " << double(elapsed) / double(loops) << " cycles " << std::endl;

    std::cout << std::endl;

    start = rdtsc();
    for (size_t i = 0; i < loops; i++) {
        *x = 0;
        _mm_mfence();
    }
    elapsed = rdtsc() - start;

    std::cout << "_mm_mfence       : " << elapsed << std::endl
              << "average          : " << (double(elapsed) / double(loops)) << " cycles " << std::endl;

    std::cout << std::endl;

#if defined(_MSC_VER)
    start = rdtsc();
    for (size_t i = 0; i < loops; i++) {
        *x = 0;
        __faststorefence();
    }
    elapsed = rdtsc() - start;

    std::cout << "__faststorefence : " << elapsed << std::endl
              << "average          : " << (double(elapsed) / double(loops)) << " cycles " << std::endl;

    std::cout << std::endl;
#else
    start = rdtsc();
    for (size_t i = 0; i < loops; i++) {
        *x = 0;
        __sync_synchronize();
    }
    elapsed = rdtsc() - start;

    std::cout << "__sync_synchronize : " << elapsed << std::endl
              << "average            : " << (double(elapsed) / double(loops)) << " cycles " << std::endl;

    std::cout << std::endl;
#endif
}

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

    test_cpu_fences();

    jmlog::finalize();
    return 0;
}


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

#include <inttypes.h>

#include <jmlog/jmlog.h>
#include <jmlog/rdtsc.h>

using namespace jmlog;

#ifdef IMPORT_ORIGINAL_LIB
#if defined(_MSC_VER)
#ifdef NDEBUG
#pragma comment(lib, "jmlog-Release.lib")
#else
#pragma comment(lib, "jmlog-Debug.lib")
#endif
#endif
#endif // IMPORT_ORIGINAL_LIB

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

#if defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_X64))
    start = rdtsc();
    for (size_t i = 0; i < loops; i++) {
        *x = 0;
        __faststorefence();
    }
    elapsed = rdtsc() - start;

    std::cout << "__faststorefence : " << elapsed << std::endl
              << "average          : " << (double(elapsed) / double(loops)) << " cycles " << std::endl;

    std::cout << std::endl;
#elif defined(__GNUC__)
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

const char * getFileName(const char * const path)
{
    const char * startPosition = (const char *)path;
    for (const char * currentCharacter = startPosition; *currentCharacter != '\0'; ++currentCharacter) {
        if (*currentCharacter == '\\' || *currentCharacter == '/') {
            startPosition = currentCharacter;
        }
    }

    if (startPosition != path) {
        ++startPosition;
    }

    return startPosition;
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

    log.info("__FILE__ = %s\n\n", __FILE__);
#if defined(__GNUC__)
    log.info("__BASE_FILE__ = %s\n\n", __BASE_FILE__);
#endif
    log.info("__FUNCTION__ = %s()\n\n", __FUNCTION__);
    jmlog_info(log, "kSourceRootDirOffset = %" PRIuPTR "\n\n", kJmSourceRootDirOffset);

    const char * filename_cxx14_1 = cxx14::v1::getSourceFileName(__FILE__, "/src/", false);
    log.info("cxx14::v1::getSourceFileName() = %s\n\n", filename_cxx14_1);

    //
    // gcc: at least gcc 6.1 + -O1
    // msvc: put result into constexpr variable
    // clang: persists on not compile time evaluation
    //
    constexpr const char * filename_cxx11_1 = &__FILE__[getSourceFileNameOffset(__FILE__)];
    log.info("getSourceFileName_cxx11_1() = %s\n\n", filename_cxx11_1);

    constexpr const char * filename_cxx11_2 = getSourceFileName_cxx11(__FILE__);
    log.info("getSourceFileName_cxx11_2() = %s\n\n", filename_cxx11_2);

    //test_cpu_fences();

    jmlog::finalize();
    return 0;
}

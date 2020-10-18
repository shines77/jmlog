
#ifndef JMLOG_RDTSC_H
#define JMLOG_RDTSC_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>

#include <cstdint>
#include <cstddef>
#include <atomic>

#undef JM_HAVE_RDTSC

//
// MSVC: __cpuid: https://docs.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=vs-2019
//

//
// std::atomic_thread_fence() : lfence, sfence, mefence
//
// https://blog.csdn.net/wxj1992/article/details/103917093
// https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence
//

//
// std::atomic_signal_fence(std::memory_order_seq_cst);
//
// How to correctly use std::atomic_signal_fence()?
// https://stackoverflow.com/questions/14581090/how-to-correctly-use-stdatomic-signal-fence
// https://stackoverflow.com/questions/61819902/how-to-use-stdatomic-signal-fence-with-semaphore-and-volatile
//
// https://en.cppreference.com/w/cpp/atomic/atomic_signal_fence
//

//--------------------------------------------------------------------------
//
// About rdtsc/rdstcp and lfence/mfence
//
// Difference between rdtscp, rdtsc : memory and cpuid / rdtsc?
// See: https://stackoverflow.com/questions/12631856/difference-between-rdtscp-rdtsc-memory-and-cpuid-rdtsc#
//
// Is there any difference in between (rdtsc + lfence + rdtsc) and (rdtsc + rdtscp) in measuring execution time?
// See: https://stackoverflow.com/questions/59759596/is-there-any-difference-in-between-rdtsc-lfence-rdtsc-and-rdtsc-rdtscp
//
// /akaros/kern/arch/x86/rdtsc_test.c
// See: http://akaros.cs.berkeley.edu/lxr/#akaros/kern/arch/x86/rdtsc_test.c
//
//--------------------------------------------------------------------------

//
// See: https://github.com/vesperix/FFTW-for-ARMv7/blob/master/kernel/cycle.h
//

#if defined(__i386__) || defined(__x86_64__) || defined(__amd64__) \
 || defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64)
// Arch: i386 or amd64
#endif

#if defined(_MSC_VER) && defined(_M_IA64)
#  include <intrin.h>
#  pragma intrinsic(__getReg)
#endif // _MSC_VER && _M_IA64

#if ((defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)) || defined(_MSC_VER)) \
 && (defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64))
#  include <intrin.h>
#  pragma intrinsic(__rdtsc)
#  pragma intrinsic(__rdtscp)

#  pragma intrinsic(_mm_lfence)
#  pragma intrinsic(_mm_mfence)
#  pragma intrinsic(_mm_sfence)

#include <emmintrin.h>      // For _mm_mfence(), _mm_lfence()
#include <xmmintrin.h>     // For _mm_sfence()

#endif // _MSC_VER && _M_IX86

#if (defined(__GNUC__) || defined(__ICC) || defined(__clang__)) \
 && (defined(__i386__) || defined(__x86_64__) || defined(__amd64__))
#  include <x86intrin.h>

#include <emmintrin.h>      // For _mm_mfence(), _mm_lfence()
#include <xmmintrin.h>     // For _mm_sfence()
#endif // __GNUC__ && __i386__

namespace jmlog {

#if defined(_MSC_VER) && defined(_M_IA64) && !defined(JM_HAVE_RDTSC)

#define JM_HAVE_RDTSC  "win-ia64"

#ifdef __cplusplus
extern "C"
#endif
uint64_t __getReg(int whichReg);

static inline uint64_t rdtsc()
{
     volatile uint64_t tsc;
     std::atomic_signal_fence(std::memory_order_seq_cst);
     tsc = __getReg(3116);
     std::atomic_signal_fence(std::memory_order_seq_cst);
     return tsc;
}

#endif // _MSC_VER && _M_IA64

#if defined(__GNUC__) && defined(__ia64__) && !defined(JM_HAVE_RDTSC)

#define JM_HAVE_RDTSC  "linux-ia64"

static inline uint64_t rdtsc()
{
     uint32_t tsc;
     std::atomic_signal_fence(std::memory_order_seq_cst);
     __asm__ __volatile__ ("mov %0=ar.itc" : "=r"(tsc));
     std::atomic_signal_fence(std::memory_order_seq_cst);
     return static_cast<uint64_t>(tsc);;
}

#endif // __GNUC__ && __ia64__

#if ((defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)) || defined(_MSC_VER)) \
 && (defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64)) && !defined(JM_HAVE_RDTSC)

#define JM_HAVE_RDTSC  "win-x86-64"

//
// Windows: https://docs.microsoft.com/en-us/cpp/intrinsics/rdtsc?view=vs-2019
//
static inline uint64_t rdtsc()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    uint64_t tsc = __rdtsc();
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

static inline uint64_t rdtscp()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    unsigned int ui;
    uint64_t tsc = __rdtscp(&ui);
    (void)ui;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

#endif // _MSC_VER && _M_IX86

#if (defined(__GNUC__) || defined(__ICC) || defined(__clang__)) \
 && (defined(__i386__) || defined(__x86_64__) || defined(__amd64__)) && !defined(JM_HAVE_RDTSC)

#define JM_HAVE_RDTSC  "linux-x86-64"

// gcc intrin function
static inline uint64_t rdtsc()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    uint64_t tsc = __builtin_ia32_rdtsc();
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

static inline uint64_t rdtsc()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    unsigned int ui;
    uint64_t tsc = __builtin_ia32_rdtscp(&ui);
    (void)ui;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

#endif // __GNUC__ && __i386__

#if (defined(__GNUC__) || defined(__ICC) || defined(__clang__)) && defined(__mips__) && !defined(JM_HAVE_RDTSC)

#define JM_HAVE_RDTSC  "linux-mips"

// Arch: MIPS
static inline uint64_t rdtsc()
{
    uint32_t tsc;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    __asm_ ___volatile__("mfc0 %0,$9; nop" : "=r" (tsc));
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return static_cast<uint64_t>(tsc);
}

#endif // __GNUC__ && __i386__

#if defined(__GNUC__) && defined(__arm__) && !defined(JM_HAVE_RDTSC)

#define JM_HAVE_RDTSC  "linux-arm"

/*
 * ARM cycle counter; WARNING: many Linux distributions do not reliably enable user-mode access
 *
 * See: https://github.com/vesperix/FFTW-for-ARMv7/blob/master/kernel/cycle.h
 *
 * How to enable user-mode ARM-v8 performance counter access in linux kernel mode.
 * "PMCCNTR_EL0" register
 * See: http://ilinuxkernel.com/?p=1755
 *
 */
static __inline__ uint64_t rdtsc(void)
{
     unsigned tsc;
     static int init = 0;
     if (!init) {
         __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 2" :: "r"(1 << 31));   /* stop the cc */
         __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 0" :: "r"(5));         /* initialize */
         __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 1" :: "r"(1 << 31));   /* start the cc */
         init = 1;
     }

     std::atomic_signal_fence(std::memory_order_seq_cst);
     __asm__ __volatile__ ("mrc p15, 0, %0, c9, c13, 0" : "=r"(tsc));
     std::atomic_signal_fence(std::memory_order_seq_cst);
     return static_cast<uint64_t>(tsc);
}

#if armv8pmu_pmcr

static inline uint32_t armv8pmu_pmcr_read(void)
{
    uint32_t val;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    __asm__ __volatile__("mrs %0, pmcr_el0" : "=r" (val));
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return val;
}
 
static inline uint64_t armv8pmu_pmcr_rdtsc(void)
{
    uint64_t tsc;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    __asm__ __volatile__("isb" : : : "memory");
    __asm__ __volatile__("mrs %0, PMCCNTR_EL0" : "+r"(tsac));
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

#endif // armv8pmu_pmcr

#endif // __GNUC__ && __arm__

#if defined(_MSC_VER) && defined(_M_ARM) || defined(_M_ARM64) && !defined(JM_HAVE_RDTSC)

// TODO: rdtsc()

#endif // _MSC_VER && _M_ARM

} // namespace jmlog

#if !defined(JM_HAVE_RDTSC)

#if (defined(__GNUC__) || defined(__ICL__) || defined(__GNUC__)) && defined(__linux__))

#include <time.h>   // For clock_gettime()

namespace jmlog {

static inline uint64_t rdtsc()
{
    struct timespec ts;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    // clock_gettime() is implemented in librt, so need add -lrt compile option.
    // clock_gettime() is define in <time.h>.
    ::clock_gettime(CLOCK_MONOTONIC, &ts);
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return (ts.tv_sec * 1000000000ULL + ts.tv_nsec);
}

} // namespace jmlog

#else

namespace jmlog {

static inline uint64_t rdtsc()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return 0;
}

} // namespace jmlog

#endif // __linux__

#endif // JM_HAVE_RDTSC

//
// rdtsc + fence
//

namespace jmlog {

//
// std::atomic_thread_fence() & CPU: lfence, sfence, mfence
// https://blog.csdn.net/wxj1992/article/details/103917093
// https://blog.csdn.net/liuhhaiffeng/article/details/106493224 (more cleanly than above acticle)
// https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence
//

//
// Does it make any sense to use the LFENCE instruction on x86/x86_64 processors?
// https://stackoverflow.com/questions/20316124/does-it-make-any-sense-to-use-the-lfence-instruction-on-x86-x86-64-processors
//

//
// Why is (or isn't?) SFENCE + LFENCE equivalent to MFENCE?
// https://stackoverflow.com/questions/27627969/why-is-or-isnt-sfence-lfence-equivalent-to-mfence
//
// SFENCE + LFENCE doesn't block StoreLoad reordering, so it's not sufficient for sequential consistency.
// Only mfence (or a locked operation, or a real serializing instruction like cpuid) will do that.
//

//
// _mm_sfence vs __faststorefence
// https://stackoverflow.com/questions/12308916/mm-sfence-vs-faststorefence
//
// __faststorefence() is generate code: LOCK OR DWORD PTR [rsp], ebp, and ebp is 0 use xor ebp inst.
//

static inline uint64_t fence_rdtsc()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    // CPU: lfence
    std::atomic_thread_fence(std::memory_order_acquire);
    uint64_t tsc = rdtsc();
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

static inline uint64_t fence_rdtsc_fence()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    // CPU: lfence
    std::atomic_thread_fence(std::memory_order_acquire);
    uint64_t tsc = rdtsc();
    // CPU: lfence
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

static inline uint64_t mfence_rdtsc()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    // CPU: full-fence, is mfence or atomic operation
    std::atomic_thread_fence(std::memory_order_seq_cst);
    uint64_t tsc = rdtsc();
    std::atomic_signal_fence(std::memory_order_seq_cst);
    return tsc;
}

static inline uint64_t mfence_rdtsc_mfence()
{
    std::atomic_signal_fence(std::memory_order_seq_cst);
    // CPU: full-fence, is mfence or atomic operation
    std::atomic_thread_fence(std::memory_order_seq_cst);
    uint64_t tsc = rdtsc();
    // CPU: full-fence, is mfence or atomic operation
    std::atomic_thread_fence(std::memory_order_seq_cst);
    std::atomic_signal_fence(std::memory_order_acq_rel);
    return tsc;
}

} // namespace jmlog

#endif // JMLOG_RDTSC_H

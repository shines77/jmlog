
#include "jmlog/jmlog.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS_)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#include <Windows.h>
#undef  NOMINMAX
#else
#include <Windows.h>
#endif // NOMINMAX
#endif // WIN32_LEAN_AND_MEAN

static void jmlog_init(void)
{
    //
}

static void jmlog_exit(void)
{
    //
}

static void jmlog_thread_init()
{
    //
}

static void jmlog_thread_cleanup()
{
    //
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            jmlog_init();
            break;

        case DLL_PROCESS_DETACH:
            // If the process is about to exit, don't bother releasing any resources
            // The kernel is much better at bulk releasing then.
            if (!reserved) {
                jmlog_exit();
            }
            break;

        case DLL_THREAD_ATTACH:
#if defined(SMP) && defined(USE_TLS)
            jmlog_thread_init();
#endif
            break;

        case DLL_THREAD_DETACH:
#if defined(SMP) && defined(USE_TLS)
            jmlog_thread_cleanup();
#endif
            break;
    }

    return TRUE;
}

#endif // WIN32

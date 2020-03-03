// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <angsys.h>

ANG_EXTERN void ang_library_start();
ANG_EXTERN void ang_library_shutdown();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ang_library_start();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        ang_library_shutdown();
        break;
    }
    return TRUE;
}


// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "framework.h"
#define COMPILE_DLL_XHTYE102474
#include "DLLdefine.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
_declspec(dllexport) int __stdcall Min(int x, int y) {
    return x < y ? x : y;
}

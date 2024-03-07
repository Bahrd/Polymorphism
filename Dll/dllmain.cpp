// dllmain.cpp : Defines the entry point for the DLL application.
// dumpbin.exe /EXPORTS .\Dll.dll


#include "pch.h"
#include "dllmain.h"


template <typename T, typename U>
auto LocalAdder(T t, U u) -> decltype(t + u)
{
    return t + u;
}

extern "C" 
{ 
    int SharedAdder(int a, int b)
    { 
        return LocalAdder(a, b); 
    } 
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:  case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

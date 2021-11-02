// Polymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/dynamic-link-library

#include <iostream>
#include <windows.h>
#include <WinUser.h>

extern "C"
{
    int __declspec(dllimport)  SharedAdder(int, int);
}
//#include "..//Dll/dllmain.h"


int main()
{
    std::cout << "CT polymorphism: "
              << SharedAdder(2, 2)
              << std::endl;


    typedef int (*DLLPROC) (int, int);

    HINSTANCE hinstDLL;
    DLLPROC sharedAdder;
    BOOL fFreeDLL = false;

    hinstDLL = LoadLibrary(TEXT(R"(./Dll.dll)"));
    if (hinstDLL != NULL)
    {
        sharedAdder = (DLLPROC)GetProcAddress(hinstDLL, "SharedAdder");
        if (sharedAdder != NULL)
            std::cout << "RT polymorphism: " 
                      << sharedAdder(3, 3) << std::endl;
        fFreeDLL = FreeLibrary(hinstDLL);
    }
    return fFreeDLL == 0;
}
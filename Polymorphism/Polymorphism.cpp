// Polymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/dynamic-link-library

#include <functional>
#include <iostream>

#include <windows.h>
#include <WinUser.h>

extern "C"
{
    int __declspec(dllimport)  SharedAdder(int, int);
}

int main()
{
    std::cout << "CT polymorphism: "
              << SharedAdder(2, 2)
              << std::endl;

    using funT = int (*) (int, int);
    HINSTANCE hinstDLL;
    BOOL fFreeDLL = false;
    
    hinstDLL = LoadLibrary(TEXT(R"(./Dll.dll)"));
    if (hinstDLL != NULL)
    {
        auto f = reinterpret_cast<funT>(GetProcAddress(hinstDLL, "SharedAdder"));
        if (f != NULL)
        {
            std::cout << "RT polymorphism: " 
                      << f(3, 3) << std::endl;
        }
        fFreeDLL = FreeLibrary(hinstDLL);
    }
    return fFreeDLL == 0;
}
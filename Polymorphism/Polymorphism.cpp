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
    std::cout << "Basic CT polymorphism: "
              << SharedAdder(2, 2)
              << std::endl;

    using funT = int (*) (int, int);
    HINSTANCE hinstDLL = LoadLibrary(TEXT(R"(./Dll.dll)"));
    BOOL fFreeDLL = false;
    
    if (hinstDLL)
    {
        std::string name; std::cin >> name;
        auto f = reinterpret_cast<funT>(GetProcAddress(hinstDLL, name.c_str()));
        if (f)
            std::cout << "Ultimate RT polymorphism: " 
                      << f(3, 3) << std::endl;
        fFreeDLL = FreeLibrary(hinstDLL);
    }
    return fFreeDLL == 0;
}
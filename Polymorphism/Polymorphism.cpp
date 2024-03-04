// Polymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/dynamic-link-library

#include <future>
#include <functional>
#include <iostream>
#include <print>

#include <windows.h>
#include <WinUser.h>


extern "C" int SharedAdder(int a, int b);

int main()
{
    std::cout << "LT polymorphism: "
              << SharedAdder(1, 2) << std::endl;
    BOOL fFreeDLL = false;    
    if (HINSTANCE hinstDLL = LoadLibrary(LR"(./Dll.dll)"))
    {
        std::string name; std::cin >> name;
        {
            using funT = int (*) (int, int);
            auto f = reinterpret_cast<funT>(GetProcAddress(hinstDLL, name.c_str()));
            if (f)
            {
                auto af = std::async(std::launch::deferred, f, 1, 2);

                std::cout << "Ultimate RT polymorphism: "
                          << f(1, 2) << std::endl
                          << "Ultimate lazy RT polymorphism: "
                          << af.get() << std::endl;
            }
        }
        {
            typedef int (*funT) (int);
            auto f = reinterpret_cast<funT>(GetProcAddress(hinstDLL, name.c_str()));
            if (f)
            {
                std::println("FUBAR polymorphism: {}", f(1));
                std::cout << "FUBAR polymorphism: "
                          << f(1) << std::endl;
            }
        }
        {
            using funT = int (*) (int, int, std::string);
            auto f = reinterpret_cast<funT>(GetProcAddress(hinstDLL, name.c_str()));
            if (f)
            {
                std::println(std::cout, "FUGAZI polymorphism: {}", f(1, 2, "Have you ever tried this?"));
            }
        }
        fFreeDLL = FreeLibrary(hinstDLL);
    }
    return fFreeDLL == 0;
}
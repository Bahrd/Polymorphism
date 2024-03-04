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
    BOOL fFreeDLL = false;
    
    std::string bib, fun;
    std::cin >> bib >> fun;
    
    auto wbib = LR"(.\)" + std::wstring(bib.begin(), bib.end()) + L".dll";
    if (auto hinstDLL = LoadLibrary(wbib.c_str()))
    {
        if (auto _f = GetProcAddress(hinstDLL, fun.c_str()))
        {
            typedef unsigned int (*fun_t) (int, int);

            auto f = reinterpret_cast<fun_t>(_f);
            auto [bf, af] = std::tuple(std::bind(f, -1, 1), std::async(f, 1, -1));
            
            std::cout << "Penultimate eager RT polymorphism: " << bf() << std::endl
                      << "Ultimately lazy RT polymorphism: " << af.get() << std::endl;
        }
        using fubar_t = int (*) (int);
        if (auto f = reinterpret_cast<fubar_t>(GetProcAddress(hinstDLL, fun.c_str())))
        {
            std::println("FUBAR polymorphism  I: {}", f(1));
            std::cout << "FUBAR polymorphism II: " << f(-1) << std::endl;
        }
        using fugazi_t = long double (*) (int, int, std::string);
        if (auto f = reinterpret_cast<fugazi_t>(GetProcAddress(hinstDLL, fun.c_str())))
        {
            std::println(std::cout, "FUGAZI polymorphism: {}", f(-1, 1, "Have you ever tried this?"));
        }
        fFreeDLL = FreeLibrary(hinstDLL);
    }
    std::cout << "Ol'good LT polymorphism: " << SharedAdder(1, -1) << std::endl;
    return fFreeDLL == 0;
}
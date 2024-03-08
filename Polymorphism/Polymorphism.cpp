// Polymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/dynamic-link-library
// https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp
// About move semantics:
// https://youtu.be/Bt3zcJZIalk?t=845
// Hacker's kindergarden: 
// https://en.wikipedia.org/wiki/Stack_buffer_overflow#Exploiting_stack_buffer_overflows
// https://arstechnica.com/information-technology/2015/08/how-security-flaws-work-the-buffer-overflow/
#include <future>
#include <functional>
#include <iostream>
#include <print>

#include <windows.h>
#include <WinUser.h>

extern "C" int SharedAdder(int, int);

int main()
{    
    std::string bib, fun; int a, b;
    std::cin >> bib >> fun >> a >> b;  

    auto wbib = LR"(.\)" + std::wstring(bib.begin(), bib.end()) + L".dll";
    if (auto hinstDLL = LoadLibraryW(wbib.c_str()))
    {
        if (auto f = GetProcAddress(hinstDLL, fun.c_str()))
        {
            typedef int (*fun_t) (int, int);
            auto tf = std::move(reinterpret_cast<fun_t>(f));
            auto [bf, af] = std::pair(std::bind(tf, a, b), std::async(tf, a, b));
            
            std::cout << "Penultimate eager RT polymorphism: " << bf() << std::endl
                      << "Ultimately RT lazy polymorphism: " << af.get() << std::endl;
        }
        using fubar_t = unsigned int (*) (int);
        if (auto f = reinterpret_cast<fubar_t>(GetProcAddress(hinstDLL, fun.c_str())))
        {
            std::println("FUBAR polymorphism  I: {}", f(a + b)),
            std::wcout << L"FUBAR polymorphism II: " << f(a + b) << std::endl;
        }
        using fugazi_t = long double (*) (char, int, std::string);
        if (auto f = reinterpret_cast<fugazi_t>(GetProcAddress(hinstDLL, fun.c_str())))
        {
            std::println(std::cout, "FUGAZI polymorphism: {}", f(a, b, "Have you ever tried this?"));
        }
        return FreeLibrary(hinstDLL) == 0;
    }
    else
    {
        std::cout << "Ol'good LT polymorphism: " << SharedAdder(a, b) << std::endl;
        return 0;
    }
}
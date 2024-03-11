﻿// Polymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
    std::string _bib, _fun; int a, b;
    std::cin >> _bib >> _fun >> a >> b;  

    auto [bib, fun] = std::pair(LR"(.\)" + std::wstring(_bib.begin(), _bib.end()) + L".dll", _fun.c_str());
    if (auto hinstDLL = LoadLibraryW(bib.c_str()))
    {
        if (auto &&f = GetProcAddress(hinstDLL, fun))
        {
            typedef int (*fun_t) (int, int);
            auto &&tf = std::move(reinterpret_cast<fun_t>(f));
            auto [bf, af, alf] = std::tuple(std::bind(tf, a, b), std::async(tf, a, b), 
                                            std::async(std::launch::deferred, [&tf, &a, &b] { return tf(a, b); }));
            
            std::cout << "Ultimately RT eager p'phism: " << bf() << std::endl
                      << "Potentially lazy RT p'phism: " << af.get() << std::endl
                      << "Penultimate slacky RT p'phism: " << alf.get() << std::endl;
        }
        using fubar_t = unsigned int (*) (int);
        if (auto f = reinterpret_cast<fubar_t>(GetProcAddress(hinstDLL, fun)))
        {
            std::println("SNAFU'd p'phism  I: {}", f(a + b)),
            std::wcout << L"FUBAR'ed p'phism II: " << f(a + b) << std::endl;
        }
        using fugazi_t = long double (*) (char, int, std::string);
        if (auto f = reinterpret_cast<fugazi_t>(GetProcAddress(hinstDLL, fun)))
        {
            std::println(std::cout, "FUGAZI'fied p'phism: {}", f(a, b, "Have you ever tried this?"));
        }
        return FreeLibrary(hinstDLL) == 0;
    }
    else
    {
        return std::cout << "Good ol' LT p'phism: " << SharedAdder(a, b) << std::endl, 0;
    }
}
/** Polymorphism.cpp : a vanilla one!
    https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/dynamic-link-library
    https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp
    dumpbin /IMPORTS .\Polymorphism.exe
    About move semantics:  https://youtu.be/Bt3zcJZIalk?t=845
    Hacker's kindergarden: 
    https://en.wikipedia.org/wiki/Stack_buffer_overflow#Exploiting_stack_buffer_overflows
    https://arstechnica.com/information-technology/2015/08/how-security-flaws-work-the-buffer-overflow/
*/
#include <future>
#include <functional>
#include <iostream>
#include <print>

#include <windows.h>
#include <WinUser.h>

extern "C" int SharedAdder(int, int);

int main()
{    
    using namespace std;
    string _bib, _fun; int a, b;
    cin >> _bib >> _fun // Dll  SharedAdder  
        >> a >> b;      // 4    4

    auto [bib, fun] = pair(LR"(.\)" + wstring(_bib.begin(), _bib.end()) + L".dll", _fun.c_str());
    if (auto hinstDLL = LoadLibraryW(bib.c_str()))
    {
        if (auto &&f = GetProcAddress(hinstDLL, fun))
        {
            typedef int (*fun_t) (int, int);
            auto &&tf = move(reinterpret_cast<fun_t>(f));
            auto [bf, af, alf] = tuple(bind(tf, a, b), 
                                       async(tf, a, b), 
                                       async(launch::deferred, [&tf, &a, &b] () -> decltype (a + b)                                       
                                       { 
                                           return tf(a, b); 
                                       }));
            cout << "Ultimately RT eager p'phism:   "   << bf() << endl
                 << "Potentially lazy RT p'phism:   "   << af.get() << endl
                 << "Penultimately slacky RT p'phism: " << alf.get() << endl;
        }
        else
        {
            wcerr << L"No such function like " << fun << " in " << bib
                  << endl;
            return -1;
        }
        using fubar_t = unsigned int (*) (int);
        if (auto f = reinterpret_cast<fubar_t>(GetProcAddress(hinstDLL, fun)))
        {
            wcerr << L"FUBAR'ed p'phism  I: " << f(a + b) 
                  << endl;
        }
        using snafu_t = int (*) (unsigned int, unsigned char);
        if (auto f = reinterpret_cast<snafu_t>(GetProcAddress(hinstDLL, fun)))
        {
            println("SNAFU'ed p'phism II: {}", f(a, b));
        }
        using fugazi_t = long double (*) (char, int, string);
        if (auto f = reinterpret_cast<fugazi_t>(GetProcAddress(hinstDLL, fun)))
        {
            println(clog, "FUGAZI'fied p'phism: {}", 
                    f(a, b, "Have you ever tried this?"));
        }
        return FreeLibrary(hinstDLL) == 0;
    }
    else
    {
        return cout << "Good ol' LT p'phism: " << SharedAdder(a, b) << endl, 0;
    }
}
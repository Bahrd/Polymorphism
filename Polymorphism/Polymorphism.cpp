/** Polymorphism.cpp : a vanilla one!
    https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/dynamic-link-library
    https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp
    dumpbin /IMPORTS .\Polymorphism.exe
    About move semantics:  https://youtu.be/Bt3zcJZIalk?t=845
    Hacker's kindergarden: 
    https://en.wikipedia.org/wiki/Stack_buffer_overflow#Exploiting_stack_buffer_overflows
    https://arstechnica.com/information-technology/2015/08/how-security-flaws-work-the-buffer-overflow/

     devenv .\Polymorphism.sln /Build "Debug|x64"
*/
#include <future>
#include <functional>
#include <iostream>
#include <print>
#include <string>

#include <windows.h>
#include <WinUser.h>

// We could've included "dllmain.h" header here, but... ;)
extern "C" int SharedAdder(int, int);
extern std::string SharedAdder(std::string, std::string);

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
            // Mangling is... "like a box of chocolates. You never know what you're gonna get!" ;)
            // Never write a code like this 'if(...){...}'! Please, please!! Pleeeeease!!!
            // https://en.wikipedia.org/wiki/Name_mangling#Standardized_name_mangling_in_C++
            if (_fun.contains("@@"))
            {
                using str = string; using rope = str(*) (str, str);
                auto&& tf = move(reinterpret_cast<rope>(f));
                cout << tf(to_string(a), to_string(b)) << endl;
                return 0;
            }
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
        // You think strict type checking is obsolete and restrictive, huh? 
        // Good for ya - keep thinking that, keeeeep thinking... "Just. Like. That." [ https://youtu.be/s_NQ9yUQ6cY?t=87 ];)
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
        return cout << "Good ol' LT p'phism: " << endl
                    << SharedAdder(to_string(a), to_string(b)) << " = " << SharedAdder(a, b) << endl
                    << "Fine'n'dandy!", 0;
    }
}
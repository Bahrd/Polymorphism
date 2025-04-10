#pragma once
#include <string>

extern "C"
{
	int __declspec(dllexport) SharedAdder(int, int);
}
std::string __declspec(dllexport) SharedAdder(std::string, std::string);
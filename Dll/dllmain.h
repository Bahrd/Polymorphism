#pragma once
#include <string>

extern "C"
{
	int __declspec(dllexport) SharedAdder(int, int);
}
extern "C++"
{
	std::string __declspec(dllexport) SharedAdder(std::string, std::string);
}
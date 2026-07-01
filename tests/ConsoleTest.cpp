#include <iostream>
#include <../src/Platform/Windows/Console/ConsoleManager.h>
//using namespace AkaNetCore;

int main()
{
	RunTUI();
	WaitForSingleObject(g_hHandle, INFINITE);
	return 0;
}
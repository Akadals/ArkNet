#include <iostream>
#include <AkaNetCore/Platform/Win32/AkaNetCore.h>

#define AKANETCORE_USE_ACCEPT_EX_STRATEGY
#include <AkaNetCore/Platform/Win32/Module.h>


using namespace AkaNetCore;

int main()
{
	g_running = true;

	SetOpt(OPT_LOGGER_LOGGING_LEVEL, 2);
	SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, true);
	SetOpt(OPT_LOGGER_FILE_OUTPUT_PATH, "D:/Develop/AkaNetCore_Test_Log");
	Logger::StartWrite();
	SetOpt(OPT_MODULE_ACCEPT_RESERVATION_COUNT, 100);
	SetOpt(OPT_LOGGER_ENABLE_COLORED, true);
	SetOpt(OPT_LOGGER_TIME_FORMAT, "[%Y-%m-%d %H.%M.%S]");
	Logger::PrintAllInfo();
	Sleep(100);
	return 0;
}
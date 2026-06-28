#include <iostream>
#include <AkaNetCore/AkaNetCore.h>
#define AKANETCORE_USE_ACCEPT_EX_STRATEGY
#include <AkaNetCore/Module.h>
#include <AkaNetCore/Opt.h>

#include <AkaNetCore/CommandManager.h>


using namespace AkaNetCore;

int main()
{
	g_running = true;
	SetOpt(OPT_LOGGER_LOGGING_LEVEL, 4);
	SetOpt(OPT_LOGGER_FILE_OUTPUT_PATH, "D:/Develop/AkaNetCore_Test_Log");
	SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, true);
	SetOpt(OPT_MODULE_ACCEPT_RESERVATION_COUNT, 100);
	SetOpt(OPT_LOGGER_ENABLE_COLORED, true);
	SetOpt(OPT_LOGGER_TIME_FORMAT, "[%Y-%m-%d %H:%M:%S]");
	SetOpt(OPT_LOGGER_START_WRITE);
	Logger::Print(ALL);
	Sleep(1000);
	return 0;
} 
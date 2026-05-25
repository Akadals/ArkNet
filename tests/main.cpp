#include <iostream>
#include <AkaNetCore/AkaNetCore.h>

using namespace AkaNetCore;

int main()
{
	g_running = true;
	SetOpt(OPT_LOGGER_LOGGING_LEVEL, 1);
	SetOpt(OPT_LOGGER_TIME_FORMAT, "%Y-%m-%d %H.%M.%S");
	SetOpt(OPT_LOGGER_FILE_OUTPUT_PATH, "D:/Develop/AkaNetCore_Test_Log");
	SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, true);
	Logger::PrintAllInfo();
	Logger::StartWrite();
	Sleep(100);
	return 0;
}
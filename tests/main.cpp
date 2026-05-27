#include <iostream>
#include <AkaNetCore/AkaNetCore.h>

#include <AkaNetCore/Module.h>


using namespace AkaNetCore;

int main()
{
	SetOpt(OPT_MODULE_ACCEPT_RESERVATION_COUNT, 100);
	SetOpt(OPT_LOGGER_ENABLE_COLORED, true);
	SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, true);
	SetOpt(OPT_LOGGER_LOGGING_LEVEL, 2);
	SetOpt(OPT_LOGGER_TIME_FORMAT, "[%Y-%m-%d %H.%M.%S]");
	SetOpt(OPT_LOGGER_FILE_OUTPUT_PATH, "D:/Develop/AkaNetCore_Test_Log");
	Logger::PrintAllInfo();
	Logger::StartWrite();
	Sleep(100);
	return 0;
}
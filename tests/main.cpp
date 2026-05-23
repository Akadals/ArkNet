#include <iostream>

#include <AkaNetCore/AkaNetCore.h>

using namespace AkaNetCore;

int main()
{
	g_running = true;
	SetOpt(OPT_LOGGER_FILE_OUTPUT_PATH, std::filesystem::path("D:/Develop"));
	SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, true);
	PRINT_INFO(AKANETCORE_VERSION);
	PRINT_INFO(AKANETCORE_VERSION);
	PRINT_INFO(AKANETCORE_VERSION);
	PRINT_INFO(AKANETCORE_VERSION);
	PRINT_INFO(AKANETCORE_VERSION);
	PRINT_INFO(AKANETCORE_VERSION);

	Sleep(1000);
	return 0;
}
#include <AkaNetCore/Logger.h>
#include <AkaNetCore/Opt.h>
#include "Internal.h"
#include <Version.h>

#define DEFAULT_LOGGING_LEVEL 1
#define DEFAULT_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define DEFAULT_ENABLE_FILE_OUTPUT false
#define DEFAULT_OUTPUT_PATH std::filesystem::path("Log/")

namespace
{
	int						g_loggingLevel		= DEFAULT_LOGGING_LEVEL;

	std::string				g_timeFormat		= DEFAULT_TIME_FORMAT;
	std::filesystem::path	g_outputPath		= DEFAULT_OUTPUT_PATH;
	bool					g_enableOutput		= DEFAULT_ENABLE_FILE_OUTPUT;

	std::ofstream			g_outputFile;

	std::queue<std::string>	g_logQueue;
	HANDLE					g_worker			= NULL;

	std::mutex				g_mtx;
}
void AkaNetCore::Internal::Logger::SetOptValueImpl(uint8_t opt, const void* param, const std::type_info& type)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	{
		g_timeFormat = *static_cast <const std::string*>(param);
		PRINT_INFO(std::string("OPT_LOGGER_TIME_FORMAT has been set to ") + g_timeFormat);
		break;
	}
	case OPT_LOGGER_ENABLE_FILE_OUTPUT:
	{
		if (g_worker) PRINT_WARNING("Cannot change OPT_LOGGER_ENABLE_FILE_OUTPUT at present");
		else
		{
			g_enableOutput = *static_cast<const bool*>(param);
			PRINT_INFO(std::string("OPT_LOGGER_ENABLE_FILE_OUTPUT has been set to ") + (g_enableOutput ? "true" : "false"));
		}
		break;
	}
	case OPT_LOGGER_FILE_OUTPUT_PATH:
	{
		if (g_worker) PRINT_WARNING("Cannot change OPT_LOGGER_FILE_OUTPUT_PATH at present");
		else
		{
			g_outputPath = *static_cast<const std::filesystem::path*>(param);
			PRINT_INFO(std::string("OPT_LOGGER_FILE_OUTPUT_PATH has been set to ") + g_outputPath.string());
		}
		break;
	}
	case OPT_LOGGER_LOGGING_LEVEL:
	{
		g_loggingLevel = *static_cast<const int*>(param);
		if (g_loggingLevel > 0)
			PRINT_INFO(std::string("OPT_LOGGER_LOGGING_LEVEL has been set to ") + std::to_string(g_loggingLevel));
	}
	}
}

std::string AkaNetCore::Logger::GetTimeStr()
{
	auto now = std::chrono::system_clock::now();

	auto seconds = std::chrono::system_clock::to_time_t(now);
	tm local;
	localtime_s(&local, &seconds);

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::ostringstream oss;

	oss << '[' << std::put_time(&local, g_timeFormat.c_str())
		<< '.' << std::setw(3) << std::setfill('0') << ms.count() << "] ";

	return oss.str();
}

std::string AkaNetCore::Logger::LevelToString(LoggingLevel level)
{
	switch (level)
	{
	default:
	case LoggingLevel::LEVEL_UNKNOWN:	return "[ UNKNOWN ] ";
	case LoggingLevel::LEVEL_INFO:		return "[ INFO ] ";
	case LoggingLevel::LEVEL_DETAIL:	return "[ DETAIL ] ";
	case LoggingLevel::LEVEL_WARNING:	return "[ WARNING ] ";
	case LoggingLevel::LEVEL_ERROR:		return "[ ERROR ] ";
	case LoggingLevel::LEVEL_EXCAPTION: return "[ EXCAPTION ] ";
	case LoggingLevel::LEVEL_MESSAGE:	return "[ MESSAGE ] ";
	case LoggingLevel::LEVEL_CRASH:		return "[ CRASH ] ";
	case LoggingLevel::LEVEL_TRACE:		return "[ TRACE ] ";
	}
}

bool AkaNetCore::Logger::OpenLogFile()
{
	if (g_outputFile.is_open()) PRINT_ERROR("The log file is already open.");

	if (g_outputPath.empty())
	{
		PRINT_ERROR("Invalid output file path");
		PRINT_INFO("It is set to the default output path, Log\\");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
	}

	if (!exists(g_outputPath)) create_directories(g_outputPath);

	auto now = std::chrono::system_clock::now();

	auto t = std::chrono::system_clock::to_time_t(now);

	tm local;
	localtime_s(&local, &t);

	std::ostringstream oss;
	oss << std::put_time(&local, "%Y-%m-%d_%H-%M-%S");

	std::string str = oss.str() + ".log";

	std::filesystem::path filePath = g_outputPath / str;

	g_outputFile.open(filePath, std::ios::out | std::ios::app);

	if (!g_outputFile.is_open())
	{
		PRINT_ERROR("Cannot open " + str + " file");
		PRINT_INFO("Automatically changes the s_enableFileOutput value to false.");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
	}
	return true;
}

void AkaNetCore::Logger::StartWrite()
{
	if (g_enableOutput)
	{
		if (!OpenLogFile()) return;
		if (!g_worker) g_worker = (HANDLE)_beginthreadex(NULL, 0, WriteThread, NULL, 0, NULL);
	}
}

void AkaNetCore::Logger::EnqueueLog(LoggingLevel level, std::string message)
{
	if (g_loggingLevel == 0) return;
	std::lock_guard<std::mutex> lock(g_mtx);

	std::string msg = GetTimeStr() + LevelToString(level) + message + '\n';
	g_logQueue.push(msg);
	fputs(msg.data(), stdout);
}

void AkaNetCore::Logger::PrintAllInfo()
{
	PrintBuildInfo();
	PrintRuntimeInfo();
}

void AkaNetCore::Logger::PrintBuildInfo()
{
	PRINT_INFO(std::string("Version : ") + AKANETCORE_VERSION);
	PRINT_INFO(std::string("Build Date : ") + AKANETCORE_BUILD_DATE);

#ifdef _DEBUG
	PRINT_INFO("Build Type : Debug");
#else
	PRINT_INFO("Build Type : Release");
#endif

#ifdef _WIN64
	PRINT_INFO("Platform : Win64");
#elif _WIN32
	PRINT_INFO("Platform : Win32");
#endif

#ifdef _MSC_VER
	PRINT_INFO(std::string("Compiler : MSVC ") + std::to_string(_MSC_VER));
#endif
}

void AkaNetCore::Logger::PrintRuntimeInfo()
{
	PRINT_INFO(std::string("C++ Standard : ") + std::to_string(__cplusplus));

	PRINT_INFO(std::string("Log Output Path : ") + +"\"" + g_outputPath.string() + "\"");

	PRINT_INFO(std::string("File Output : ")
		+ (g_enableOutput ? "Enabled" : "Disabled"));
}

unsigned __stdcall AkaNetCore::Logger::WriteThread(PVOID arg)
{
	while (g_running)
	{
		std::vector<std::string> logs;

		{
			std::lock_guard<std::mutex> lock(g_mtx);
			while (!g_logQueue.empty())
			{
				logs.push_back(g_logQueue.front());
				g_logQueue.pop();
			}
		}
		for (auto& v : logs) g_outputFile << v;

		g_outputFile.flush();
		Sleep(1);
	}
	g_outputFile.close();
	CloseHandle(g_worker);
	return 0;
}

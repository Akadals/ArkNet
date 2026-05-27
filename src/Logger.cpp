#include <AkaNetCore/Logger.h>
#include <AkaNetCore/Opt.h>
#include <Version.h>
#include "Internal.h"

#define DEFAULT_LOGGING_LEVEL 2
#define DEFAULT_TIME_FORMAT "[%Y-%m-%d %H.%M.%S]"
#define DEFAULT_ENABLE_FILE_OUTPUT false
#define DEFAULT_ENABLE_COLORED true
#define DEFAULT_OUTPUT_PATH std::filesystem::path("Log/")

namespace
{
	enum Color : uint8_t
	{
		BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
		LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN,
		LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
	};

	int						g_loggingLevel		= DEFAULT_LOGGING_LEVEL;

	std::string				g_timeFormat		= DEFAULT_TIME_FORMAT;
	std::filesystem::path	g_outputPath		= DEFAULT_OUTPUT_PATH;
	bool					g_enableOutput		= DEFAULT_ENABLE_FILE_OUTPUT;
	bool					g_enableColored		= DEFAULT_ENABLE_COLORED;

	std::ofstream			g_outputFile;

	std::queue<std::string>	g_logQueue;
	HANDLE					g_worker			= NULL;

	std::mutex				g_mtx;
}
void AkaNetCore::Internal::Logger::SetOptValue(UINT32 opt, const void* param)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	{
		g_timeFormat = *static_cast <const std::string*>(param);
		PRINT_DETAIL(std::string("OPT_LOGGER_TIME_FORMAT has been set to ") + g_timeFormat);
		break;
	}
	case OPT_LOGGER_ENABLE_FILE_OUTPUT:
	{
		if (g_worker) PRINT_WARNING("Cannot change OPT_LOGGER_ENABLE_FILE_OUTPUT at present");
		else
		{
			g_enableOutput = *static_cast<const bool*>(param);
			PRINT_DETAIL(std::string("OPT_LOGGER_ENABLE_FILE_OUTPUT has been set to ") + (g_enableOutput ? "true" : "false"));
		}
		break;
	}
	case OPT_LOGGER_FILE_OUTPUT_PATH:
	{
		if (g_worker) PRINT_WARNING("Cannot change OPT_LOGGER_FILE_OUTPUT_PATH at present");
		else
		{
			g_outputPath = *static_cast<const std::filesystem::path*>(param);
			PRINT_DETAIL(std::string("OPT_LOGGER_FILE_OUTPUT_PATH has been set to ") + g_outputPath.string());
		}
		break;
	}
	case OPT_LOGGER_LOGGING_LEVEL:
	{
		g_loggingLevel = *static_cast<const UINT8*>(param);
		if (g_loggingLevel > 0)
			PRINT_DETAIL(std::string("OPT_LOGGER_LOGGING_LEVEL has been set to ") + std::to_string(g_loggingLevel));
		break;
	}
	case OPT_LOGGER_ENABLE_COLORED:
	{
		g_enableColored = *static_cast<const bool*>(param);
		PRINT_DETAIL(std::string("OPT_LOGGER_ENABLE_COLORED has been set to ") + (g_enableColored ? "true" : "false"));
		break;
	}
	}
}

void AkaNetCore::Internal::Logger::SetColor(uint8_t color)
{
	if (!g_enableColored) return;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(color));
}

std::string AkaNetCore::Internal::Logger::GetTimeStr()
{
	auto now = std::chrono::system_clock::now();

	auto seconds = std::chrono::system_clock::to_time_t(now);
	tm local;
	localtime_s(&local, &seconds);

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::ostringstream oss;

	oss << std::put_time(&local, g_timeFormat.c_str()) << (g_timeFormat.empty() ? "" : " ");

	return oss.str();
}

std::string AkaNetCore::Internal::Logger::LevelToString(uint8_t level)
{
	switch (static_cast<LoggingLevel>(level))
	{
	default:
	case LoggingLevel::LEVEL_UNKNOWN:	return "UNKNOWN";
	case LoggingLevel::LEVEL_INFO:		return "INFO";
	case LoggingLevel::LEVEL_DETAIL:	return "DETAIL";
	case LoggingLevel::LEVEL_WARNING:	return "WARNING";
	case LoggingLevel::LEVEL_ERROR:		return "ERROR";
	case LoggingLevel::LEVEL_EXCAPTION: return "EXCAPTION";
	case LoggingLevel::LEVEL_MESSAGE:	return "MESSAGE";
	case LoggingLevel::LEVEL_CRASH:		return "CRASH";
	case LoggingLevel::LEVEL_TRACE:		return "TRACE";
	}
}

uint8_t AkaNetCore::Internal::Logger::LevelToColor(uint8_t level)
{
	switch (static_cast<LoggingLevel>(level))
	{
	default:
	case LoggingLevel::LEVEL_UNKNOWN:	return WHITE;
	case LoggingLevel::LEVEL_INFO:		return LIGHTGREEN;
	case LoggingLevel::LEVEL_DETAIL:	return LIGHTBLUE;
	case LoggingLevel::LEVEL_WARNING:	return YELLOW;
	case LoggingLevel::LEVEL_ERROR:		return LIGHTRED;
	case LoggingLevel::LEVEL_EXCAPTION: return RED;
	case LoggingLevel::LEVEL_MESSAGE:	return LIGHTMAGENTA;
	case LoggingLevel::LEVEL_CRASH:		return BLUE;
	case LoggingLevel::LEVEL_TRACE:		return CYAN;
	}
}

bool AkaNetCore::Logger::OpenLogFile()
{
	if (g_outputFile.is_open()) PRINT_ERROR("The log file is already open.", ANCERRINVALSTREAM);

	if (g_outputPath.empty())
	{
		PRINT_ERROR("Invalid output file path", ANCERRBADPATH);
		PRINT_INFO("It is set to the default output path, Log\\");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
	}

	try
	{
	if (!exists(g_outputPath)) create_directories(g_outputPath);
	}
	catch (std::exception e)
	{
		PRINT_EXCAPTION("Invalid output file path", ANCERRBADPATH);
		return false;
	}

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
		PRINT_EXCAPTION("Cannot open " + str + " file", ANCERRINVALSTREAM);
		PRINT_INFO("Automatically changes the s_enableFileOutput value to false.");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
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

void AkaNetCore::Logger::EnqueueLog(LoggingLevel level, std::string message, DWORD err)
{
	if (g_loggingLevel == 0) return;
	if (g_loggingLevel < 2 && level == LoggingLevel::LEVEL_DETAIL) return;
	
	if (level == LoggingLevel::LEVEL_ERROR ||
		level == LoggingLevel::LEVEL_EXCAPTION) Internal::Core::SetErrNo(err);

	std::lock_guard<std::mutex> lock(g_mtx);

	std::string timeStr = Internal::Logger::GetTimeStr();
	std::string levelStr = Internal::Logger::LevelToString((uint8_t)level);

	std::string msg = timeStr + "[ " + levelStr + " ] " + message + '\n';

	fputs(timeStr.data(), stdout);
	fputs("[ ", stdout);
	Internal::Logger::SetColor(Internal::Logger::LevelToColor((uint8_t)level));
	fputs(levelStr.data(), stdout);
	Internal::Logger::SetColor(LIGHTGRAY);
	fputs(" ] ", stdout);
	fputs(message.data(), stdout);
	fputc('\n', stdout);

	if (!(g_running && !g_enableOutput)) g_logQueue.push(msg);
	else
	{
		while (!g_logQueue.empty()) g_logQueue.pop();
	}
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

DWORD_EX AkaNetCore::Logger::GetLastError()
{
	return DWORD_EX(Internal::Core::GetErrNo());
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
		Sleep(100);
	}
	g_outputFile.close();
	CloseHandle(g_worker);
	return 0;
}

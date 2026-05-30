#include <AkaNetCore/Logger.h>
#include <AkaNetCore/Opt.h>
#include <AkaNetCore/Core.h>
#include "../Internal.h"

#define ANCBASEERR	100000

#define ANCERRGOOD				0
#define ANCERRINVALTYPE			(ANCBASEERR + 1)
#define ANCERRINVALINCL			(ANCBASEERR + 2)
#define ANCERRINVALSESH			(ANCBASEERR + 3)
#define ANCERRBADHANDSHAKE		(ANCBASEERR + 4)
#define ANCERRBADCONN			(ANCBASEERR + 5)
#define ANCERRFIREWALLBLOCK		(ANCBASEERR + 6)
#define ANCERRAUTHORITY			(ANCBASEERR + 7)
#define ANCERRBADIP				(ANCBASEERR + 8)
#define ANCERRBADPORT			(ANCBASEERR + 9)
#define ANCERRNOTALLOW			(ANCBASEERR + 10)
#define ANCERRBADREQ			(ANCBASEERR + 11)
#define ANCERRBADTLSCRT			(ANCBASEERR + 12)
#define ANCERRBADTLSKEY			(ANCBASEERR + 13)
#define ANCERRBADTRAFFIC		(ANCBASEERR + 14)
#define ANCERRBADHARDWARE		(ANCBASEERR + 15)
#define ANCERRINVALSTREAM		(ANCBASEERR + 16)
#define ANCERRBADPATH			(ANCBASEERR + 17)
#define ANCERRFILEOPEN			(ANCBASEERR + 18)
#define ANCERROPTLOCK			(ANCBASEERR + 19)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)
#define ANCERR			(ANCBASEERR + 0)


namespace
{
	enum Color : uint8_t
	{
		BLACK, BLUE, GREEN, CYAN,
		RED, MAGENTA, BROWN, LIGHTGRAY,
		DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
		LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
	};

#pragma region Logging Level
	constexpr uint32_t LogLevel0 =
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_UNKNOWN) |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_EXCAPTION);

	constexpr uint32_t LogLevel1 =
		LogLevel0 |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_INFO) |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_ERROR) |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_CRASH);

	constexpr uint32_t LogLevel2 =
		LogLevel1 |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_DETAIL) |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_WARNING);

	constexpr uint32_t LogLevel3 =
		LogLevel2 |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_MESSAGE);

	constexpr uint32_t LogLevel4 =
		LogLevel3 |
		static_cast<uint32_t>(AkaNetCore::Logger::LogType::LEVEL_TRACE);

	constexpr std::array<uint32_t, 5> mask =
	{
		LogLevel0,
		LogLevel1,
		LogLevel2,
		LogLevel3,
		LogLevel4
	};
#pragma endregion

	uint8_t							iLoggingLevel	= DEFAULT_LOGGING_LEVEL;

	std::string						sTimeFormat		= DEFAULT_TIME_FORMAT;
	std::filesystem::path			pOutputPath		= std::filesystem::path(DEFAULT_OUTPUT_PATH);
	std::string						sOutputPath		= DEFAULT_OUTPUT_PATH;
	bool							bEnableOutput	= DEFAULT_ENABLE_FILE_OUTPUT;
	bool							bEnableColored	= DEFAULT_ENABLE_COLORED;

	std::ofstream					outputFile;

	std::queue<std::string>			logQueue;

	DWORD							iLastError		= ANCERRGOOD;

	std::mutex						mtx;

	std::string GetTimeStr()
	{
		auto now = std::chrono::system_clock::now();

		auto seconds = std::chrono::system_clock::to_time_t(now);
		tm local;
		localtime_s(&local, &seconds);

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		std::ostringstream oss;

		oss << std::put_time(&local, sTimeFormat.c_str()) << (sTimeFormat.empty() ? "" : " ");

		return oss.str();
	}
	std::string GetTypeStr(AkaNetCore::Logger::LogType type)
	{
		switch (type)
		{
		default:
		case AkaNetCore::Logger::LogType::LEVEL_UNKNOWN:	return "UNKNOWN";
		case AkaNetCore::Logger::LogType::LEVEL_INFO:		return "INFO";
		case AkaNetCore::Logger::LogType::LEVEL_DETAIL:		return "DETAIL";
		case AkaNetCore::Logger::LogType::LEVEL_WARNING:	return "WARNING";
		case AkaNetCore::Logger::LogType::LEVEL_ERROR:		return "ERROR";
		case AkaNetCore::Logger::LogType::LEVEL_EXCAPTION:	return "EXCAPTION";
		case AkaNetCore::Logger::LogType::LEVEL_MESSAGE:	return "MESSAGE";
		case AkaNetCore::Logger::LogType::LEVEL_CRASH:		return "CRASH";
		case AkaNetCore::Logger::LogType::LEVEL_TRACE:		return "TRACE";
		}
	}
	Color GetTypeColor(AkaNetCore::Logger::LogType type)
	{
		switch (type)
		{
		default:
		case AkaNetCore::Logger::LogType::LEVEL_UNKNOWN:	return WHITE;
		case AkaNetCore::Logger::LogType::LEVEL_INFO:		return LIGHTGREEN;
		case AkaNetCore::Logger::LogType::LEVEL_DETAIL:		return LIGHTBLUE;
		case AkaNetCore::Logger::LogType::LEVEL_WARNING:	return YELLOW;
		case AkaNetCore::Logger::LogType::LEVEL_ERROR:		return LIGHTRED;
		case AkaNetCore::Logger::LogType::LEVEL_EXCAPTION:	return RED;
		case AkaNetCore::Logger::LogType::LEVEL_MESSAGE:	return LIGHTMAGENTA;
		case AkaNetCore::Logger::LogType::LEVEL_CRASH:		return BLUE;
		case AkaNetCore::Logger::LogType::LEVEL_TRACE:		return CYAN;
		}
	}
	void SetColor(Color color)
	{
		if (!bEnableColored) return;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(color));
	}
	unsigned __stdcall OutputThread(PVOID arg)
	{
		if (!AkaNetCore::Logger::OpenLogFile())
		{
			outputFile.close();
			CloseHandle(AkaNetCore::Logger::hWorker);
			return 0;
		}
		while (AkaNetCore::g_running)
		{
			std::vector<std::string> logs;

			{
				std::lock_guard<std::mutex> lock(mtx);
				while (!logQueue.empty())
				{
					logs.push_back(logQueue.front());
					logQueue.pop();
				}
			}
			for (auto& v : logs) outputFile << v;

			outputFile.flush();
			Sleep(10);
		}
		outputFile.close();
		CloseHandle(AkaNetCore::Logger::hWorker);
		return 0;
	}
	void StartThread()
	{
		if (AkaNetCore::Logger::hWorker) return;
		AkaNetCore::Logger::hWorker = (HANDLE)_beginthreadex(NULL, 0, OutputThread, NULL, 0, NULL);
	}

}
void AkaNetCore::Internal::Logger::SetOptValue(UINT32 opt, const void* param)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	{
		AkaNetCore::Logger::RuntimeInfo::time_format = 
			(sTimeFormat = *static_cast <const std::string*>(param)).data();
		LOG_DETAIL(std::string("OPT_LOGGER_TIME_FORMAT has been set to ") + sTimeFormat);
		break;
	}
	case OPT_LOGGER_ENABLE_FILE_OUTPUT:
	{
		if (AkaNetCore::Logger::hWorker)
		{
			LOG_WARNING("Cannot change OPT_LOGGER_ENABLE_FILE_OUTPUT at present");
			SetLastError(ANCERROPTLOCK);
		}
		else
		{
			AkaNetCore::Logger::RuntimeInfo::output_enable = 
				(bEnableOutput = *static_cast<const bool*>(param)) ? "Enabled" : "Disabled";
			LOG_DETAIL(std::string("OPT_LOGGER_ENABLE_FILE_OUTPUT has been set to ") + (bEnableOutput ? "true" : "false"));
		}
		break;
	}
	case OPT_LOGGER_START_WRITE:
	{
		if (!bEnableOutput)
		{
			LOG_WARNING("Cannot start writing because OPT_LOGGER_ENABLE_FILE_OUTPUT is false");
			SetLastError(ANCERROPTLOCK);
		}
		else if (!AkaNetCore::Logger::hWorker) StartThread();
		break;
	}
	case OPT_LOGGER_FILE_OUTPUT_PATH:
	{
		if (AkaNetCore::Logger::hWorker)
		{
			LOG_WARNING("Cannot change OPT_LOGGER_FILE_OUTPUT_PATH at present");
			SetLastError(ANCERROPTLOCK);
		}
		else
		{
			sOutputPath = (pOutputPath = *static_cast<const std::filesystem::path*>(param)).string();
			AkaNetCore::Logger::RuntimeInfo::output_path = sOutputPath.data();
			LOG_DETAIL(std::string("OPT_LOGGER_FILE_OUTPUT_PATH has been set to ") + pOutputPath.string());
		}
		break;
	}
	case OPT_LOGGER_LOGGING_LEVEL:
	{
		UINT8 level = *static_cast<const UINT8*>(param);
		AkaNetCore::Logger::RuntimeInfo::logging_level = iLoggingLevel = level > 4 ? 4 : level;
		LOG_DETAIL(std::string("OPT_LOGGER_LOGGING_LEVEL has been set to ") + std::to_string(iLoggingLevel));
		break;
	}
	case OPT_LOGGER_ENABLE_COLORED:
	{
		bEnableColored = *static_cast<const bool*>(param);
		AkaNetCore::Logger::RuntimeInfo::colored_enable = bEnableColored ? "Enabled" : "Disabled";
		LOG_DETAIL(std::string("OPT_LOGGER_ENABLE_COLORED has been set to ") + (bEnableColored ? "true" : "false"));
		break;
	}
	}
}

bool AkaNetCore::Logger::OpenLogFile()
{
	if (outputFile.is_open())
	{
		LOG_ERROR("The log file is already open.");
		SetLastError(ANCERRFILEOPEN);
	}

	if (pOutputPath.empty())
	{
		LOG_ERROR("Invalid output file path");
		SetLastError(ANCERRBADPATH);
		LOG_INFO("It is set to the default output path, Log\\");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
	}

	try
	{
	if (!exists(pOutputPath)) create_directories(pOutputPath);
	}
	catch (std::exception e)
	{
		LOG_EXCAPTION("Invalid output file path");
		SetLastError(ANCERRBADPATH);
		return false;
	}

	auto now = std::chrono::system_clock::now();

	auto t = std::chrono::system_clock::to_time_t(now);

	tm local;
	localtime_s(&local, &t);

	std::ostringstream oss;
	oss << std::put_time(&local, "%Y-%m-%d_%H-%M-%S");

	std::string str = oss.str() + ".log";

	std::filesystem::path filePath = pOutputPath / str;

	outputFile.open(filePath, std::ios::out | std::ios::app);

	if (!outputFile.is_open())
	{
		LOG_EXCAPTION("Cannot open " + str + " file");
		SetLastError(ANCERRFILEOPEN);
		LOG_INFO("Automatically changes the s_enableFileOutput value to false.");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
	}
	return true;
}

void AkaNetCore::Logger::Print(LogType type, std::string log)
{
	if (iLoggingLevel == 0) return;
	if (!(mask[iLoggingLevel] & static_cast<uint32_t>(type))) return;

	std::lock_guard<std::mutex> lock(mtx);

	std::string timeStr = GetTimeStr();
	std::string typeStr = GetTypeStr(type);
	std::string logStr = timeStr + "[ " + typeStr + " ] " + log + '\n';

	fputs(timeStr.data(), stdout);
	fputs("[ ", stdout);
	SetColor(GetTypeColor(type));
	fputs(typeStr.data(), stdout);
	SetColor(LIGHTGRAY);
	fputs(" ] ", stdout);
	fputs(log.data(), stdout);
	fputc('\n', stdout);

	if (bEnableOutput) logQueue.push(logStr);
}
void AkaNetCore::Logger::Print(InfoType type)
{
	if (HARDWARE_INFO & type)
	{
		//Hardware info
	}
	if (BUILD_INFO & type)
	{
		LOG_INFO("Build Info");
		LOG_INFO(std::string("Version : ")
			+ BuildInfo::version);
		LOG_INFO(std::string("Build Date : ")
			+ BuildInfo::build_date);
		LOG_INFO(std::string("Build Type : ")
			+ BuildInfo::build_type);
		LOG_INFO(std::string("Platform : ")
			+ BuildInfo::platform);
		LOG_INFO(std::string("C++ Standard : ")
			+ BuildInfo::standard);
		LOG_INFO(std::string("Compiler : ")
			+ BuildInfo::compiler);
	}
	if (RUNTIME_INFO & type)
	{
		LOG_INFO("Runtime Info");
		LOG_INFO(std::string("Logging Level : ")
			+ std::to_string(RuntimeInfo::logging_level));
		LOG_INFO(std::string("Log Time Format : ")
			+ RuntimeInfo::time_format);
		LOG_INFO(std::string("Enable Colored : ")
			+ RuntimeInfo::colored_enable);
		LOG_INFO(std::string("Output Enable : ")
			+ RuntimeInfo::output_enable);
		LOG_INFO(std::string("Output Path : ")
			+ RuntimeInfo::output_path);
	}
}

DWORD AkaNetCore::Logger::GetLastError()
{
	return iLastError;
}
void AkaNetCore::Internal::Logger::SetLastError(DWORD err)
{
	iLastError = err;
}
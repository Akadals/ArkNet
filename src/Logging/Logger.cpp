#include <AkaNetCore/Logger.h>
#include <AkaNetCore/Opt.h>
#include <AkaNetCore/Core.h>
#include "../Internal.h"

//Test
#include "../Platform/Windows/Console/ConsoleManager.h"

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
	std::string						sOutputPath		= DEFAULT_OUTPUT_PATH;
	bool							bEnableOutput	= DEFAULT_ENABLE_FILE_OUTPUT;
	bool							bEnableColored	= DEFAULT_ENABLE_COLORED;

	std::ofstream					outputFile;

	std::queue<std::string>			logQueue;

	DWORD							iLastError		= ANCERRGOOD;

	std::mutex						mtx;

	HANDLE							hWorker			= NULL;

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
			CloseHandle(hWorker);
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
		CloseHandle(hWorker);
		return 0;
	}
	void StartThread()
	{
		if (hWorker) return;
		hWorker = (HANDLE)_beginthreadex(NULL, 0, OutputThread, NULL, 0, NULL);
	}
	const char* GetCpuName()
	{
		static char result[128] = {};
		char brand[49] = {};

#if defined(_MSC_VER)

		int cpuInfo[4];

		__cpuid(cpuInfo, 0x80000002);
		memcpy(brand + 0, cpuInfo, 16);

		__cpuid(cpuInfo, 0x80000003);
		memcpy(brand + 16, cpuInfo, 16);

		__cpuid(cpuInfo, 0x80000004);
		memcpy(brand + 32, cpuInfo, 16);

#else

		unsigned int eax, ebx, ecx, edx;

		__get_cpuid(0x80000002, &eax, &ebx, &ecx, &edx);
		memcpy(brand + 0, &eax, 4);
		memcpy(brand + 4, &ebx, 4);
		memcpy(brand + 8, &ecx, 4);
		memcpy(brand + 12, &edx, 4);

		__get_cpuid(0x80000003, &eax, &ebx, &ecx, &edx);
		memcpy(brand + 16, &eax, 4);
		memcpy(brand + 20, &ebx, 4);
		memcpy(brand + 24, &ecx, 4);
		memcpy(brand + 28, &edx, 4);

		__get_cpuid(0x80000004, &eax, &ebx, &ecx, &edx);
		memcpy(brand + 32, &eax, 4);
		memcpy(brand + 36, &ebx, 4);
		memcpy(brand + 40, &ecx, 4);
		memcpy(brand + 44, &edx, 4);

#endif
		unsigned threads = std::thread::hardware_concurrency();

		snprintf(
			result,
			sizeof(result),
			"%s %u Thread",
			brand,
			threads
		);

		return result;
	}
	const char* GetRamSize()
	{
		static char result[32];

		uint64_t ramBytes = 0;

#ifdef _WIN32

		GetPhysicallyInstalledSystemMemory(&ramBytes);

#else

		ramBytes =
			static_cast<uint64_t>(sysconf(_SC_PHYS_PAGES)) *
			static_cast<uint64_t>(sysconf(_SC_PAGE_SIZE));

#endif

		snprintf(
			result,
			sizeof(result),
			"%llu MB",
			ramBytes / 1024
		);

		return result;
	}

	inline static const char* registry[AkaNetCore::InfoKey::COUNT] =
	{
		AKANETCORE_VERSION,
		"4.0.0", //need including openssl
		"35.0", //need including protobuf
		AKANETCORE_BUILD_DATE,
#ifdef _DEBUG
		"Debug",
#else
		"Release",
#endif
#ifdef _MSC_VER
			"MSVC",
#endif
#if CPP_STANDARD >= 202302L
			"C++23",
#elif CPP_STANDARD >= 202002L
			 "C++20",
#elif CPP_STANDARD >= 201703L
			"C++17",
#elif CPP_STANDARD >= 201402L
			"C++14",
#elif CPP_STANDARD >= 201103L
			"C++11",
#else
			"Pre-C++11",
#endif
		GET_STRING(DEFAULT_LOGGING_LEVEL),
		DEFAULT_TIME_FORMAT,
		DEFAULT_OUTPUT_PATH,
		DEFAULT_ENABLE_FILE_OUTPUT ? "Enabled" : "Disabled",
		DEFAULT_ENABLE_COLORED ? "Enabled" : "Disabled",
		GetCpuName(),
		GetRamSize(),
		"Windows 11 Pro 25H2",
		"x64",
		"DESKTOP-ABCDEF",
		"1234",
	};

}
void AkaNetCore::Internal::Logger::SetOptValue(UINT32 opt, const void* param)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	{
		registry[TIME_FORMAT] =
			(sTimeFormat = *static_cast <const std::string*>(param)).data();
		LOG_DETAIL(std::string("OPT_LOGGER_TIME_FORMAT has been set to ") + sTimeFormat);
		break;
	}
	case OPT_LOGGER_ENABLE_FILE_OUTPUT:
	{
		if (hWorker) LOG_WARNING("Cannot change OPT_LOGGER_ENABLE_FILE_OUTPUT at present", ANCERROPTLOCK);
		else
		{
			registry[OUTPUT_ENABLE] =
				(bEnableOutput = *static_cast<const bool*>(param)) ? "Enabled" : "Disabled";
			LOG_DETAIL(std::string("OPT_LOGGER_ENABLE_FILE_OUTPUT has been set to ") + (bEnableOutput ? "true" : "false"));
		}
		break;
	}
	case OPT_LOGGER_START_WRITE:
	{
		if (!bEnableOutput) LOG_WARNING("Cannot start writing because OPT_LOGGER_ENABLE_FILE_OUTPUT is false", ANCERROPTLOCK);
		else if (!hWorker) StartThread();
		break;
	}
	case OPT_LOGGER_FILE_OUTPUT_PATH:
	{
		if (hWorker) LOG_WARNING("Cannot change OPT_LOGGER_FILE_OUTPUT_PATH at present", ANCERROPTLOCK);
		else
		{
			sOutputPath = *static_cast<const std::string*>(param);
			registry[OUTPUT_PATH] = sOutputPath.data();
			LOG_DETAIL(std::string("OPT_LOGGER_FILE_OUTPUT_PATH has been set to ") + sOutputPath);
		}
		break;
	}
	case OPT_LOGGER_LOGGING_LEVEL:
	{
		UINT8 level = *static_cast<const UINT8*>(param);
		iLoggingLevel = level > 4 ? 4 : level;
		static char buf[8];
		snprintf(buf, sizeof(buf), "%d", iLoggingLevel);
		registry[LOGGING_LEVEL] = buf;
		LOG_DETAIL(std::string("OPT_LOGGER_LOGGING_LEVEL has been set to ") + std::to_string(iLoggingLevel));
		break;
	}
	case OPT_LOGGER_ENABLE_COLORED:
	{
		bEnableColored = *static_cast<const bool*>(param);
		registry[COLORED_ENABLE] = bEnableColored ? "Enabled" : "Disabled";
		LOG_DETAIL(std::string("OPT_LOGGER_ENABLE_COLORED has been set to ") + (bEnableColored ? "true" : "false"));
		break;
	}
	}
}

bool AkaNetCore::Logger::OpenLogFile()
{
	if (outputFile.is_open()) LOG_ERROR("The log file is already open.", ANCERRFILEOPEN);

	auto path = std::filesystem::path(sOutputPath);

	if (path.empty())
	{
		LOG_ERROR("Invalid output file path", ANCERRBADPATH);
		LOG_INFO("It is set to the default output path, Log\\");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
	}

	try
	{
	if (!exists(path)) create_directories(path);
	}
	catch (std::exception e)
	{
		LOG_EXCAPTION("Invalid output file path", ANCERRBADPATH);
		return false;
	}

	auto now = std::chrono::system_clock::now();

	auto t = std::chrono::system_clock::to_time_t(now);

	tm local;
	localtime_s(&local, &t);

	std::ostringstream oss;
	oss << std::put_time(&local, "%Y-%m-%d_%H-%M-%S");

	std::string str = oss.str() + ".log";

	std::filesystem::path filePath = path / str;

	outputFile.open(filePath, std::ios::out | std::ios::app);

	if (!outputFile.is_open())
	{
		LOG_EXCAPTION("Cannot open " + str + " file", ANCERRFILEOPEN);
		LOG_INFO("Automatically changes the OPT_LOGGER_ENABLE_FILE_OUTPUT value to false.");
		SetOpt(OPT_LOGGER_ENABLE_FILE_OUTPUT, false);
		return false;
	}
	return true;
}

void AkaNetCore::Logger::Print(LogType type, std::string log, DWORD lastErr)
{
	if (iLoggingLevel == 0) return;
	if (!(mask[iLoggingLevel] & static_cast<uint32_t>(type))) return;

	std::lock_guard<std::mutex> lock(mtx);

	auto errMask =
		static_cast<uint32_t>(LogType::LEVEL_WARNING) |
		static_cast<uint32_t>(LogType::LEVEL_ERROR) |
		static_cast<uint32_t>(LogType::LEVEL_EXCAPTION);

	if (errMask & static_cast<uint32_t>(type)) Internal::Logger::SetLastError(lastErr);

	std::string timeStr = GetTimeStr();
	std::string typeStr = GetTypeStr(type);
	std::string logStr = timeStr + "[ " + typeStr + " ] " + log + '\n';

	//fputs(timeStr.data(), stdout);
	//fputs("[ ", stdout);
	//SetColor(GetTypeColor(type));
	//fputs(typeStr.data(), stdout);
	//SetColor(LIGHTGRAY);
	//fputs(" ] ", stdout);
	//fputs(log.data(), stdout);
	//fputc('\n', stdout);

	//RegistLog(logStr);
	if (bEnableOutput) logQueue.push(logStr);
}
void AkaNetCore::Logger::Print(InfoType type)
{
	if (HARDWARE_INFO & type)
	{
		LOG_INFO("========== Hardware Info =========");
		LOG_INFO(std::string("CPU : ")
			+ registry[CPU_INFO]);
		LOG_INFO(std::string("RAM : ")
			+ registry[RAM_INFO]);
	}
	if (SYS_INFO & type)
	{
		LOG_INFO("=========== System Info ==========");
		LOG_INFO(std::string("OS : ")
			+ registry[OS_INFO]);
		LOG_INFO(std::string("Architecture  : ")
			+ registry[ARCHITECTURE_INFO]);
		LOG_INFO(std::string("Hostname  : ")
			+ registry[HOST_INFO]);
		LOG_INFO(std::string("Process ID  : ")
			+ registry[PROCESS_ID]);
	}
	if (BUILD_INFO & type)
	{
		LOG_INFO("=========== Build Info ===========");
		LOG_INFO(std::string("Version : ")
			+ registry[VERSION]);
		LOG_INFO(std::string("OpenSSL Version : ")
			+ registry[OPENSSL_VERSION]);
		LOG_INFO(std::string("Protobuf Version : ")
			+ registry[PROTOBUF_VERSION]);
		LOG_INFO(std::string("Build Date : ")
			+ registry[BUILD_DATE]);
		LOG_INFO(std::string("Build Type : ")
			+ registry[BUILD_TYPE]);
		LOG_INFO(std::string("C++ Standard : ")
			+ registry[STANDARD]);
		LOG_INFO(std::string("Compiler : ")
			+ registry[COMPILER]);
	}
	if (RUNTIME_INFO & type)
	{
		LOG_INFO("========== Runtime Info ==========");
		LOG_INFO(std::string("Logging Level : ")
			+ registry[LOGGING_LEVEL]);
		LOG_INFO(std::string("Log Time Format : ")
			+ registry[TIME_FORMAT]);
		LOG_INFO(std::string("Enable Colored : ")
			+ registry[COLORED_ENABLE]);
		LOG_INFO(std::string("Output Enable : ")
			+ registry[OUTPUT_ENABLE]);
		LOG_INFO(std::string("Output Path : ")
			+ registry[OUTPUT_PATH]);
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

const char* AkaNetCore::GetRegistry(InfoKey key)
{
	return registry[key];
}
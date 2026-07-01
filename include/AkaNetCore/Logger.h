#pragma once

#include <AkaNetCore/State.h>

#include <stdint.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <iostream>
#include <fstream>
#include <ostream>
#include <thread>
#include <queue>
#include <iomanip>
#include <sstream>
#include <vector>
#include <array>
#include <Version.h>

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <cpuid.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define ISCSTR inline static constexpr const char*

#if defined(_MSVC_LANG)
#define CPP_STANDARD _MSVC_LANG
#else
#define CPP_STANDARD __cplusplus
#endif
#define TO_STRING(x) #x
#define GET_STRING(x) TO_STRING(x)

#define DEFAULT_LOGGING_LEVEL_STR STR(DEFAULT_LOGGING_LEVEL)

#define ERROR_LOG(type,log)			AkaNetCore::Logger::Print(type, log);
#define LOG_UNKNOWN(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_UNKNOWN,	log)
#define LOG_INFO(log)			AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_INFO,		log)
#define LOG_DETAIL(log)			AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_DETAIL,	log)
#define LOG_WARNING(log, err)	AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_WARNING,	log, err)
#define LOG_WARNING(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_WARNING,	log)
#define LOG_ERROR(log, err)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_ERROR,		log, err)
#define LOG_ERROR(log)			AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_ERROR,		log)
#define LOG_EXCAPTION(log, err)	AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_EXCAPTION, log, err)
#define LOG_EXCAPTION(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_EXCAPTION, log)
#define LOG_MESSAGE(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_MESSAGE,	log)
#define LOG_CRASH(log)			AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_CRASH,		log)
#define LOG_TRACE(log)			AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_TRACE,		log)

#define DEFAULT_LOGGING_LEVEL 4
#define DEFAULT_TIME_FORMAT "[%Y-%m-%d %H:%M:%S]"
#define DEFAULT_ENABLE_FILE_OUTPUT true
#define DEFAULT_ENABLE_COLORED true
#define DEFAULT_OUTPUT_PATH "Log/"

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
#define ANCERRCMDREGIST			(ANCBASEERR + 20)
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

namespace AkaNetCore
{
	enum InfoType : uint8_t
	{
		BUILD_INFO		= 1 << 0,
		RUNTIME_INFO	= 1 << 1,
		HARDWARE_INFO	= 1 << 2,
		SYS_INFO		= 1 << 3,
		ALL = BUILD_INFO | RUNTIME_INFO | HARDWARE_INFO | SYS_INFO
	};
	enum InfoKey : uint8_t
	{
		VERSION,
		OPENSSL_VERSION,
		PROTOBUF_VERSION,
		BUILD_DATE,
		BUILD_TYPE,
		COMPILER,
		STANDARD,

		LOGGING_LEVEL,
		TIME_FORMAT,
		OUTPUT_PATH,
		OUTPUT_ENABLE,
		COLORED_ENABLE,

		CPU_INFO,
		RAM_INFO,
		OS_INFO, 
		ARCHITECTURE_INFO,
		HOST_INFO,
		PROCESS_ID,

		COUNT
	};
	const char* GetRegistry(InfoKey key);
	namespace Logger
	{
		enum class LogType : uint32_t
		{
			LEVEL_UNKNOWN	= 1 << 0,
			LEVEL_INFO		= 1 << 1,
			LEVEL_DETAIL	= 1 << 2,
			LEVEL_WARNING	= 1 << 3,
			LEVEL_ERROR		= 1 << 4,
			LEVEL_EXCAPTION = 1 << 5,
			LEVEL_MESSAGE	= 1 << 6,
			LEVEL_CRASH		= 1 << 7,
			LEVEL_TRACE		= 1 << 8,
			LEVEL_HELP		= 1 << 9,
		};

		bool OpenLogFile();

		void Print(LogType type, std::string log = "", DWORD lastErr = 0);
		void Print(InfoType type);

		DWORD GetLastError();
	}
}
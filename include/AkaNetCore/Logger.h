#pragma once

#include <AkaNetCore/State.h>

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

#define ISCSTR inline static constexpr const char*

#if defined(_MSVC_LANG)
#define CPP_STANDARD _MSVC_LANG
#else
#define CPP_STANDARD __cplusplus
#endif

#define LOG(type,log)		AkaNetCore::Logger::Print(type, log);
#define LOG_UNKNOWN(log)	AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_UNKNOWN, log)
#define LOG_INFO(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_INFO, log)
#define LOG_DETAIL(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_DETAIL, log)
#define LOG_WARNING(log)	AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_WARNING, log)
#define LOG_ERROR(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_ERROR, log)
#define LOG_EXCAPTION(log)	AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_EXCAPTION, log)
#define LOG_MESSAGE(log)	AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_MESSAGE, log)
#define LOG_CRASH(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_CRASH, log)
#define LOG_TRACE(log)		AkaNetCore::Logger::Print(AkaNetCore::Logger::LogType::LEVEL_TRACE, log)

#define DEFAULT_LOGGING_LEVEL 4
#define DEFAULT_TIME_FORMAT "[%Y-%m-%d %H.%M.%S]"
#define DEFAULT_ENABLE_FILE_OUTPUT true
#define DEFAULT_ENABLE_COLORED true
#define DEFAULT_OUTPUT_PATH "Log/"

namespace AkaNetCore
{
	enum InfoType : uint8_t
	{
		BUILD_INFO		= 1 << 0,
		RUNTIME_INFO	= 1 << 1,
		HARDWARE_INFO	= 1 << 2,
		ALL = BUILD_INFO | RUNTIME_INFO | HARDWARE_INFO
	};
	namespace Logger
	{
		inline HANDLE hWorker = NULL;
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
			LEVEL_TRACE		= 1 << 8
		};

		struct BuildInfo
		{
			ISCSTR version = AKANETCORE_VERSION;
			ISCSTR build_date = AKANETCORE_BUILD_DATE;

#ifdef _DEBUG
			ISCSTR build_type = "Debug";
#else
			ISCSTR build_type = "Release";
#endif

#ifdef _WIN64
			ISCSTR platform = "Win64";
#elif _WIN32
			ISCSTR platform = "Win32";
#else
			ISCSTR platform = "Linux";
#endif

#ifdef _MSC_VER
			ISCSTR compiler = "MSVC";
#endif


#if CPP_STANDARD >= 202302L
			ISCSTR standard = "C++23";
#elif CPP_STANDARD >= 202002L
			ISCSTR standard = "C++20";
#elif CPP_STANDARD >= 201703L
			ISCSTR standard = "C++17";
#elif CPP_STANDARD >= 201402L
			ISCSTR standard = "C++14";
#elif CPP_STANDARD >= 201103L
			ISCSTR standard = "C++11";
#else
			ISCSTR standard = "Pre-C++11";
#endif
		};

		struct RuntimeInfo
		{
			inline static UINT8 logging_level			= DEFAULT_LOGGING_LEVEL;
			inline static const char* time_format		= DEFAULT_TIME_FORMAT;
			inline static const char* output_path		= DEFAULT_OUTPUT_PATH;
			inline static const char* output_enable		= DEFAULT_ENABLE_COLORED ? "Enabled" : "Disabled";
			inline static const char* colored_enable	= DEFAULT_ENABLE_FILE_OUTPUT ? "Enabled" : "Disabled";
		};
		bool OpenLogFile();

		void Print(LogType type, std::string log = "");
		void Print(InfoType type);

		DWORD GetLastError();
	}
}
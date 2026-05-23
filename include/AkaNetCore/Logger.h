#pragma once
#include <AkaNetCore/Core.h>

#include <string>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <iostream>
#include <fstream>
#include <ostream>
#include <thread>
#include <queue>
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include <vector>

#define PRINT_UNKNOWN(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_UNKNOWN,message)
#define PRINT_INFO(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_INFO,message)
#define PRINT_DETAIL(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_DETAIL,message)
#define PRINT_WARNING(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_WARNING,message)
#define PRINT_ERROR(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_ERROR,message)
#define PRINT_EXCAPTION(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_EXCAPTION,message)
#define PRINT_MESSAGE(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_MESSAGE,message)
#define PRINT_CRASH(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_CRASH,message)
#define PRINT_TRACE(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_TRACE,message)

#define DEFAULT_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define DEFAULT_ENABLE_FILE_OUTPUT false
#define DEFAULT_OUTPUT_PATH "Log/"

using namespace std::chrono;
using namespace std::filesystem;

using TIME_POINT = system_clock::time_point;

namespace AkaNetCore
{
	enum class LoggingLevel : uint8_t
	{
		LEVEL_UNKNOWN, LEVEL_INFO, LEVEL_DETAIL, LEVEL_WARNING,
		LEVEL_ERROR, LEVEL_EXCAPTION, LEVEL_MESSAGE, LEVEL_CRASH, LEVEL_TRACE
	};
	namespace Logger
	{
		inline std::string				s_timeFormat		= DEFAULT_TIME_FORMAT;
		inline bool						s_enableFileOutput	= DEFAULT_ENABLE_FILE_OUTPUT;
		inline path						s_OutputPath		= DEFAULT_OUTPUT_PATH;

		inline std::ofstream			s_logFile;
		inline std::queue<std::string>	s_logQueue;
		inline HANDLE					s_worker			= NULL;

		std::string GetTime();
		std::string LevelToString(LoggingLevel level);

		bool OpenLogFile();
		void Startup();
		void EnqueueLog(LoggingLevel level, std::string message);

		unsigned __stdcall WriteThread(PVOID arg);
	};
}
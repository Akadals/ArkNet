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

#define PRINT_UNKNOWN(message) Logger::EnqueueLog(LoggingLevel::LEVEL_UNKNOWN,message)
#define PRINT_INFO(message) Logger::EnqueueLog(LoggingLevel::LEVEL_INFO,message)
#define PRINT_DETAIL(message) Logger::EnqueueLog(LoggingLevel::LEVEL_DETAIL,message)
#define PRINT_WARNING(message) Logger::EnqueueLog(LoggingLevel::LEVEL_WARNING,message)
#define PRINT_ERROR(message) Logger::EnqueueLog(LoggingLevel::LEVEL_ERROR,message)
#define PRINT_EXCAPTION(message) Logger::EnqueueLog(LoggingLevel::LEVEL_EXCAPTION,message)
#define PRINT_MESSAGE(message) Logger::EnqueueLog(LoggingLevel::LEVEL_MESSAGE,message)
#define PRINT_CRASH(message) Logger::EnqueueLog(LoggingLevel::LEVEL_CRASH,message)
#define PRINT_TRACE(message) Logger::EnqueueLog(LoggingLevel::LEVEL_TRACE,message)

#define DEFAULT_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define DEFAULT_ENABLE_FILE_OUTPUT false
#define DEFAULT_OUTPUT_PATH "Log/"

using namespace chrono;
using namespace filesystem;

using TIME_POINT = system_clock::time_point;

namespace AkaNetCore
{
	enum class LoggingLevel : UINT8
	{
		LEVEL_UNKNOWN, LEVEL_INFO, LEVEL_DETAIL, LEVEL_WARNING,
		LEVEL_ERROR, LEVEL_EXCAPTION, LEVEL_MESSAGE, LEVEL_CRASH, LEVEL_TRACE
	};
	class Logger
	{
	public:
		inline static const char*			s_timeFormat		= DEFAULT_TIME_FORMAT;
		inline static bool					s_enableFileOutput	= DEFAULT_ENABLE_FILE_OUTPUT;
		inline static path					s_OutputPath		= DEFAULT_OUTPUT_PATH;
	private:
		inline static TIME_POINT			s_now = system_clock::now();
		inline static ofstream				s_logFile;
		inline static queue<const char*>	s_logQueue;
		inline static mutex					s_mtx;
		inline static thread				s_worker;
	private:
		static const char* GetTime();
		static const char* LevelToString(LoggingLevel level);
		static bool OpenLogFile();
	public:
		static void Startup();
		static void EnqueueLog(LoggingLevel level, const char* message);
	private:
		static void WriteThread();
	};
}
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
#include <variant>
#include <type_traits>
#include <utility> 
#include <queue>
#include <Windows.h>
#include <iomanip>
#include <sstream>

#define LOGGER_OPT_TIME_FORMAT 0
#define LOGGER_OPT_ENABLE_FILE_OUTPUT 1
#define LOGGER_OPT_LOGGING_DEPTH 2

#define PRINT_UNKNOWN(message) Logger::EnqueueLog(LoggingLevel::LEVEL_UNKNOWN,message)
#define PRINT_INFO(message) Logger::EnqueueLog(LoggingLevel::LEVEL_INFO,message)
#define PRINT_DETAIL(message) Logger::EnqueueLog(LoggingLevel::LEVEL_DETAIL,message)
#define PRINT_WARNING(message) Logger::EnqueueLog(LoggingLevel::LEVEL_WARNING,message)
#define PRINT_ERROR(message) Logger::EnqueueLog(LoggingLevel::LEVEL_ERROR,message)
#define PRINT_EXCAPTION(message) Logger::EnqueueLog(LoggingLevel::LEVEL_EXCAPTION,message)
#define PRINT_MESSAGE(message) Logger::EnqueueLog(LoggingLevel::LEVEL_MESSAGE,message)
#define PRINT_CRASH(message) Logger::EnqueueLog(LoggingLevel::LEVEL_CRASH,message)
#define PRINT_TRACE(message) Logger::EnqueueLog(LoggingLevel::LEVEL_TRACE,message)

using namespace chrono;
using namespace filesystem;


namespace AkaNetCore
{
	inline atomic<bool> g_running;

	enum class LoggingLevel : UINT8
	{
		LEVEL_UNKNOWN, LEVEL_INFO, LEVEL_DETAIL, LEVEL_WARNING,
		LEVEL_ERROR, LEVEL_EXCAPTION, LEVEL_MESSAGE, LEVEL_CRASH, LEVEL_TRACE
	};
	class Logger
	{
	private:
		using TIME_POINT = system_clock::time_point;
		using OptParam = variant<const char*, bool, int>;

		inline static int s_loggingDepth = 0;

		inline static TIME_POINT s_now = system_clock::now();
		inline static const char* s_timeFormat = "%Y-%m-%d %H:%M:%S";

		inline static bool s_enableFileOutput = false;
		inline static path s_OutputPath = "";
		inline static ofstream s_logFile;
		inline static queue<const char*> s_logQueue;

		inline static mutex s_mtx;

		inline static thread s_worker;
	private:
		static void UpdateCurrentTime();
		static const char* GetTime();
		static const char* LevelToString(LoggingLevel level);
		static bool OpenLogFile();
	public:
		static void Startup();
		static void SetOpt(UINT8 opt, OptParam param);
		static void EnqueueLog(LoggingLevel level, const char* message);
	private:
		static void WriteThread();
	};
}
#pragma once
#include <AkaNetCore/Platform/Win32/Core.h>
#include <AkaNetCore/Platform/Win32/Opt.h>

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
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include <vector>

#define PRINT_UNKNOWN(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_UNKNOWN,message)
#define PRINT_INFO(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_INFO,message)
#define PRINT_DETAIL(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_DETAIL,message)
#define PRINT_WARNING(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_WARNING,message)
#define PRINT_ERROR(message, err) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_ERROR,message, err)
#define PRINT_EXCAPTION(message, err) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_EXCAPTION,message, err)
#define PRINT_MESSAGE(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_MESSAGE,message)
#define PRINT_CRASH(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_CRASH,message)
#define PRINT_TRACE(message) AkaNetCore::Logger::EnqueueLog(AkaNetCore::LoggingLevel::LEVEL_TRACE,message)

namespace AkaNetCore
{
	enum class LoggingLevel : uint8_t
	{
		LEVEL_UNKNOWN, LEVEL_INFO, LEVEL_DETAIL, LEVEL_WARNING,
		LEVEL_ERROR, LEVEL_EXCAPTION, LEVEL_MESSAGE, LEVEL_CRASH, LEVEL_TRACE
	};
	namespace Logger
	{
		bool OpenLogFile();
		void StartWrite();
		void EnqueueLog(LoggingLevel level, std::string message, DWORD err = ANCERRGOOD);
		void PrintAllInfo();
		void PrintBuildInfo();
		void PrintRuntimeInfo();

		DWORD_EX GetLastError();

		unsigned __stdcall WriteThread(PVOID arg);
	};
}
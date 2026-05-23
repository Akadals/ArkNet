#include <AkaNetCore/Logger.h>

std::string AkaNetCore::Logger::GetTime()
{
	auto now = system_clock::now();

	auto seconds = system_clock::to_time_t(now);
	tm local;
	localtime_s(&local, &seconds);

	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

	std::ostringstream oss;

	oss << '[' << std::put_time(&local, s_timeFormat.c_str())
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
	if (s_logFile.is_open()) PRINT_ERROR("The log file is already open.");

	if (s_OutputPath.empty())
	{
		PRINT_ERROR("Invalid output file path");
		PRINT_INFO("It is set to the default output path, Log\\");
		s_enableFileOutput = false;
		return false;
	}

	if (!exists(s_OutputPath)) create_directories(s_OutputPath);

	auto now = system_clock::now();

	auto t = system_clock::to_time_t(now);

	tm local;
	localtime_s(&local, &t);

	std::ostringstream oss;
	oss << std::put_time(&local, "%Y-%m-%d_%H-%M-%S");

	std::string str = oss.str() + ".log";

	path filePath = s_OutputPath / str;

	s_logFile.open(filePath, std::ios::out | std::ios::app);

	if (!s_logFile.is_open())
	{
		PRINT_ERROR("Cannot open " + str + " file");
		PRINT_INFO("Automatically changes the s_enableFileOutput value to false.");
		s_enableFileOutput = false;
	}
	PRINT_INFO("The output file path has been set to " + std::filesystem::absolute(filePath).string());
	return true;
}

void AkaNetCore::Logger::Startup()
{
	if (s_enableFileOutput)
	{
		if (!OpenLogFile()) return;
		if (!s_worker) s_worker = (HANDLE)_beginthreadex(NULL, 0, WriteThread, NULL, 0, NULL);
	}
}

void AkaNetCore::Logger::EnqueueLog(LoggingLevel level, std::string message)
{
	std::lock_guard<std::mutex> lock(s_mtx);

	std::string msg = GetTime() + LevelToString(level) + message + '\n';
	s_logQueue.push(msg);
	fputs(msg.data(), stdout);
}

unsigned __stdcall AkaNetCore::Logger::WriteThread(PVOID arg)
{
	while (g_running)
	{
		std::vector<std::string> logs;

		{
			std::lock_guard<std::mutex> lock(s_mtx);
			while (!s_logQueue.empty())
			{
				logs.push_back(s_logQueue.front());
				s_logQueue.pop();
			}
		}
		for (auto& v : logs) s_logFile << v;

		s_logFile.flush();
		Sleep(1);
	}
	return 0;
}

#include <AkaNetCore/Logger.h>

void AkaNetCore::Logger::UpdateCurrentTime()
{
	s_now = system_clock::now();
}

const char* AkaNetCore::Logger::GetTime()
{
	static string str;

	UpdateCurrentTime();

	auto seconds = system_clock::to_time_t(s_now);
	tm* local = localtime(&seconds);

	auto ms = duration_cast<milliseconds>(s_now.time_since_epoch()) % 1000;

	ostringstream oss;

	oss << '[' << put_time(local, s_timeFormat)
		<< '.' << setw(3) << setfill('0') << ms.count() << "] ";
	str = oss.str();

	return str.c_str();
}

const char* AkaNetCore::Logger::LevelToString(LoggingLevel level)
{
	switch (level)
	{
	default: case LoggingLevel::LEVEL_UNKNOWN: return "[UNKNOWN] ";
	case LoggingLevel::LEVEL_INFO: return "[INFO] ";
	case LoggingLevel::LEVEL_DETAIL: return "[DETAIL] ";
	case LoggingLevel::LEVEL_WARNING: return "[WARNING] ";
	case LoggingLevel::LEVEL_ERROR: return "[ERROR] ";
	case LoggingLevel::LEVEL_EXCAPTION: return "[EXCAPTION] ";
	case LoggingLevel::LEVEL_MESSAGE: return "[MESSAGE] ";
	case LoggingLevel::LEVEL_CRASH: return "[CRASH] ";
	case LoggingLevel::LEVEL_TRACE: return "[TRACE] ";
	}
}

bool AkaNetCore::Logger::OpenLogFile()
{
	if (s_logFile.is_open()) PRINT_ERROR("The log file is already open.");
	if (!exists(s_OutputPath))
	{
		if (s_OutputPath == "")
		{
			PRINT_ERROR("s_OutputPath value is invalid.");
			PRINT_INFO("Automatically changes the s_enableFileOutput value to false.");
			s_enableFileOutput = false;
			return;
		}
		create_directories(s_OutputPath);

		UpdateCurrentTime();

		auto t = system_clock::to_time_t(s_now);

		tm* local = localtime(&t);

		ostringstream oss;
		oss << put_time(local, "%Y-%m-%d");

		string str = oss.str() + ".log";

		path filePath = s_OutputPath / str;

		s_logFile.open(filePath, ios::out | ios::app);

		if (!s_logFile.is_open())
		{
			auto msg = "Cannot open" + str + " file";
			PRINT_EXCAPTION(msg.c_str());
			PRINT_INFO("Automatically changes the s_enableFileOutput value to false.");
			s_enableFileOutput = false;
		}
	}
}

void AkaNetCore::Logger::Startup()
{
	if (s_enableFileOutput)
	{
		if (!OpenLogFile()) return;
		if (s_worker.joinable())
		{
			PRINT_EXCAPTION("Startup has already run.");
			return;
		}

		s_worker = thread(WriteThread);
	}
}

void AkaNetCore::Logger::SetOpt(UINT8 opt, OptParam param)
{
	lock_guard<mutex> lock(s_mtx);
	if (s_worker.joinable())
	{
		PRINT_EXCAPTION("SetOpt cannot be called after Startup");
		return;
	}
	visit([&](auto&& v)
		{
		using T = decay_t<decltype(v)>;

		if constexpr (is_same_v<T, const char*>) s_timeFormat = v;
		else if constexpr (is_same_v<T, bool>) s_enableFileOutput = v;
		else if constexpr (is_same_v<T, int>) s_loggingDepth = v;
		}, param);
}

void AkaNetCore::Logger::EnqueueLog(LoggingLevel level, const char* message)
{
	lock_guard<mutex> lock(s_mtx);

	string msg = (string)GetTime() + LevelToString(level) + message + '\n';
	s_logQueue.push(msg.data());
	fputs(msg.data(), stdout);
}

void AkaNetCore::Logger::WriteThread()
{
	if (!s_enableFileOutput) return;
	while (g_running)
	{
		vector<string> logs;

		{
			lock_guard<mutex> lock(s_mtx);

			while (!s_logQueue.empty())
			{
				logs.push_back(s_logQueue.front());
				s_logQueue.pop();
			}
		}
		for (auto& v : logs) s_logFile << v;

		s_logFile.flush();
		Sleep(100);
	}
}

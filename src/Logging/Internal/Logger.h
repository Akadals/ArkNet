#pragma once
#include <queue>
#include <string>
#include <thread>
#include <psapi.h>
#include <vector>
#include <sstream>

#define ERROR_LOG(x) auto_error_logging(x)

class Log
{
private:
	inline static std::queue<std::string> m_log;
public:
    void operator+=(const std::string& t_other);
};

inline Log internalLog;

std::vector<std::wstring> split(std::wstring t_input, wchar_t t_delimiter);

std::string get_last_error_as_string();

void auto_error_logging(BOOL t_cond);

std::wstring get_cpu_name();
DWORD get_cpu_core_count();
DWORD get_cpu_thread_count();
double get_memory_usage(SHORT t_unit = 0);
double get_memory_size(SHORT t_unit = 0);
#pragma once
#include <queue>
#include <string>

#define ERROR_LOG(x) auto_error_logging(x)

class Log
{
private:
	inline static std::queue<std::string> m_log;
public:
    void operator+=(const std::string& t_other);
};

inline Log internalLog;

std::string GetLastErrorAsString();

void auto_error_logging(BOOL cond);
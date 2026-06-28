#include "Logger.h"



void Log::operator+=(const std::string& t_other)
{
	m_log.push(t_other);
}

std::string GetLastErrorAsString()
{
    DWORD err = GetLastError();
    if (err == 0) return "";

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);

    return message;
}

void auto_error_logging(BOOL cond)
{
    if (!cond) internalLog += GetLastErrorAsString();
}

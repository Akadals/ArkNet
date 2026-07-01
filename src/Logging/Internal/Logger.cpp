#include "Logger.h"

namespace
{
    std::chrono::steady_clock::time_point g_lastMemoryUsageUpdate =
        std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point g_lastMemorySizeUpdate =
        std::chrono::steady_clock::now();
}

void Log::operator+=(const std::string& t_other)
{
	m_log.push(t_other);
}

std::vector<std::wstring> split(std::wstring t_input, wchar_t t_delimiter)
{
    std::vector<std::wstring> result;
    std::wistringstream iss(t_input);
    std::wstring token;

    while (std::getline(iss, token, t_delimiter))
    {
        if (!token.empty()) result.push_back(token);
        else result.push_back(L" ");
    }
    return result;
}

std::string get_last_error_as_string()
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

void auto_error_logging(BOOL t_cond)
{
    if (!t_cond) internalLog += get_last_error_as_string();
}

std::wstring get_cpu_name()
{
    char brand[49] = {};
    int cpuInfo[4];

    __cpuid(cpuInfo, 0x80000002);
    memcpy(brand + 0, cpuInfo, 16);

    __cpuid(cpuInfo, 0x80000003);
    memcpy(brand + 16, cpuInfo, 16);

    __cpuid(cpuInfo, 0x80000004);
    memcpy(brand + 32, cpuInfo, 16);

    return std::wstring(brand, brand + strlen(brand)) +
        L" (" +
        std::to_wstring(get_cpu_core_count()) +
        L"C " +
        std::to_wstring(get_cpu_thread_count()) +
        L"T)";
}
DWORD get_cpu_core_count()
{
    DWORD length = 0;

    GetLogicalProcessorInformationEx(
        RelationProcessorCore,
        nullptr,
        &length);

    std::vector<BYTE> buffer(length);

    auto info =
        reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(
            buffer.data());

    if (!GetLogicalProcessorInformationEx(
        RelationProcessorCore,
        info,
        &length))
    {
        return 0;
    }

    DWORD cores = 0;

    BYTE* ptr = buffer.data();
    BYTE* end = ptr + length;

    while (ptr < end)
    {
        auto p =
            reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);

        if (p->Relationship == RelationProcessorCore)
            ++cores;

        ptr += p->Size;
    }

    return cores;
}

DWORD get_cpu_thread_count()
{
    return std::thread::hardware_concurrency();
}

double get_memory_usage(SHORT t_unit)
{
    PROCESS_MEMORY_COUNTERS pmc;
    ERROR_LOG(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)));

    switch (t_unit)
    {
    case 0:
        return pmc.WorkingSetSize;
    case 1:
        return pmc.WorkingSetSize / 1024.0;
    case 2:
        return pmc.WorkingSetSize / 1024.0 / 1024.0;
    case 3:
        return pmc.WorkingSetSize / 1024.0 / 1024.0 / 1024.0;
    }
}

double get_memory_size(SHORT t_unit)
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    switch (t_unit)
    {
    case 0:
        return memInfo.ullTotalPhys;
    case 1:
        return memInfo.ullTotalPhys / 1024.0;
    case 2:
        return memInfo.ullTotalPhys / 1024.0 / 1024.0;
    case 3:
        return memInfo.ullTotalPhys / 1024.0 / 1024.0 / 1024.0;
    }
}

std::wostringstream memUsage;
std::wstring get_memory_usage_string(SHORT t_unit)
{
    auto now = std::chrono::steady_clock::now();
    if (!memUsage.str().empty() &&
        now - g_lastMemoryUsageUpdate < std::chrono::milliseconds(500))
    {
        return memUsage.str();
    }
    memUsage.clear();
    memUsage.str(L"");
    memUsage << std::fixed << std::setprecision(1) << get_memory_usage(1);
    g_lastMemoryUsageUpdate = now;
    return memUsage.str();
}

std::wstring get_memory_size_string(SHORT t_unit)
{
    std::wostringstream memSize;
    memSize << std::fixed << std::setprecision(1) << get_memory_size(2);
    return memSize.str();
}

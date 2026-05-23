#pragma once

#ifdef _WIN32

#ifdef AKANETCORE_SHARED

#ifdef AKANETCORE_EXPORTS
#define AKA_API __declspec(dllexport)
#else
#define AKA_API __declspec(dllimport)
#endif

#else
#define AKA_API
#endif

#else
#define AKA_API
#endif

#define MAX_BUF_SIZE 0x800

#include <WinSock2.h>
#include <MSWSock.h>

#include <atomic>
#include <mutex>

namespace AkaNetCore
{
	inline std::atomic<bool> g_running;
	inline std::mutex s_mtx;
}
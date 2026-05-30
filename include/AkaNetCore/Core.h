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

#define TO_STRING(x) #x

#define MAX_BUF_SIZE 2048


#include <WinSock2.h>
#include <MSWSock.h>

#include <atomic>
#include <string>
#include <mutex>

namespace AkaNetCore
{
	inline std::atomic<bool> g_running;

	inline SOCKET listenSock;
	inline SOCKADDR_IN listenAdr;
}
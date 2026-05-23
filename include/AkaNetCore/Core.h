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

#include <variant>
#include <type_traits>
#include <utility> 

using namespace std;

#define MAX_BUF_SIZE 0x800

#define OPT_LOGGER_TIME_FORMAT 0
#define OPT_LOGGER_ENABLE_FILE_OUTPUT 1
#define OPT_LOGGER_FILE_OUTPUT_PATH 2

namespace AkaNetCore
{
	using OptParam = variant<const char*, bool, int, float>;

	inline atomic<bool> g_running;

	inline void SetOpt(UINT8 opt, OptParam param);
}
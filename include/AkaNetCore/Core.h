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

using namespace std;

namespace AkaNetCore
{
}
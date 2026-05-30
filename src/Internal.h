#pragma once
#include <type_traits>

namespace AkaNetCore::Internal
{
	namespace Logger
	{
		void SetLastError(DWORD err);
		void SetOptValue(UINT32 opt, const void* param);
	}
}
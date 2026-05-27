#pragma once
#include <type_traits>

namespace AkaNetCore
{
	namespace Internal
	{
		namespace Core
		{
			DWORD GetErrNo();
			void SetErrNo(DWORD val);
		}
		namespace Logger
		{
			void SetColor(uint8_t color);
			std::string GetTimeStr();
			std::string LevelToString(uint8_t level);
			uint8_t LevelToColor(uint8_t level);
			void SetOptValue(UINT32 opt, const void* param);
		}
		namespace Module
		{
			namespace Accept
			{
				void SetOptValue(UINT32 opt, const void* param);
			}
		}
	}
}
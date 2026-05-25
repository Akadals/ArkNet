#pragma once
#include <type_traits>

namespace AkaNetCore
{
	namespace Internal
	{
		namespace Logger
		{
			void SetOptValueImpl(uint8_t opt, const void* param, const std::type_info& type);
			template<typename T>
			inline void SetOptValue(uint8_t opt, const T& param)
			{ SetOptValueImpl(opt, &param, typeid(T)); }
		}
	}
}
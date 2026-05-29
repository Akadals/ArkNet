#include <AkaNetCore/Platform/Win32/Core.h>

namespace AkaNetCore
{
	class Serializer
	{
	public:
		template <typename T>
		static char* Serialize(const T& data);
		template <typename T>
		static T Deserialize(const char* serialized);
	};
}
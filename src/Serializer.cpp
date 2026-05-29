#include <AkaNetCore/Platform/Win32/Serializer.h>

template<typename T>
char* AkaNetCore::Serializer::Serialize(const T& data)
{
	return "";
}

template<typename T>
T AkaNetCore::Serializer::Deserialize(const char* serialized)
{
	return T();
}

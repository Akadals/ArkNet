#include <AkaNetCore/Platform/Win32/Job.h>

AkaNetCore::Job::Job()
{
	Init();
}

void AkaNetCore::Job::Init()
{
	std::ZeroMemory(&data, sizeof(std::vector<char>));
	header = 0;
}

#pragma once
#include <AkaNetCore/Platform/Win32/Core.h>
#include <AkaNetCore/Platform/Win32/LockFreePool.h>

#include <vector>

namespace AkaNetCore
{
	typedef class Job JOB, * PJOB;
	class Job : public LockFreePoolAvailable
	{
	public:
		std::vector<char>	data = {};
		uint32_t			header = NULL;
	public:
		Job();
		void Init() override;
	};
}
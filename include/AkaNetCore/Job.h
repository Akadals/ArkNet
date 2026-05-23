#pragma once
#include <AkaNetCore/Core.h>
#include <AkaNetCore/LockFreePool.h>

#include <vector>

namespace AkaNetCore
{
	typedef class Job JOB, * PJOB;
	class Job : public LockFreePoolAvailable
	{
	public:
		std::vector<char>	data = {};
		uint32_t		header = NULL;
	public:
		Job();
		void Init() override;
	};
}
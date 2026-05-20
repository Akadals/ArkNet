#pragma once
#include <AkaNetCore/Core.h>
#include <AkaNetCore/LockFreePool.h>

namespace AkaNetCore
{
	typedef class Job JOB, * PJOB;
	class Job : public LockFreePoolAvailable
	{
	public:
		vector<char>	data = {};
		uint32_t		header = NULL;
	public:
		Job();
		void Init() override;
	};
}
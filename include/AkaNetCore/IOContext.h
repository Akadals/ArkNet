#pragma once
#include <AkaNetCore/Core.h>
#include <AkaNetCore/Session.h>
#include <AkaNetCore/OverlappedExpend.h>
#include <AkaNetCore/LockFreePool.h>

namespace AkaNetCore
{
	typedef class IOContext IOCONTEXT, * PIOCONTEXT;

	class alignas(64) IOContext : public LockFreePoolAvailable
	{
	public:
		OVERLAPPED_EX		overlappedEx = {};
		PSESSION			owner = nullptr;
	public:
		IOContext();
		void Init() override;
		void Init(PSESSION owner, IOType ioType = IO_READING);
	};
}
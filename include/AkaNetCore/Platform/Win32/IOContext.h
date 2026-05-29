#pragma once
#include <AkaNetCore/Platform/Win32/Core.h>
#include <AkaNetCore/Platform/Win32/OverlappedExpend.h>
#include <AkaNetCore/Platform/Win32/LockFreePool.h>

namespace AkaNetCore
{
	typedef class Session SESSION, * PSESSION;

	typedef class IOContext IOCONTEXT, * PIOCONTEXT;

	class alignas(64) IOContext : public LockFreePoolAvailable
	{
	public:
		OVERLAPPED_EX		overlappedEx		= {};
		PSESSION			owner				= NULL;
	public:
		IOContext();
		void Init() override;
		void Init(PSESSION owner, IOType ioType = IO_READING);
	};
}
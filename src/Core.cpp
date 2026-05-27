#include <AkaNetCore/Core.h>
#include "Internal.h"

namespace
{
	std::atomic<DWORD> g_lastErrNo = ANCERRGOOD;
}

DWORD AkaNetCore::Internal::Core::GetErrNo()
{
	return g_lastErrNo.load(std::memory_order_relaxed);
}

void AkaNetCore::Internal::Core::SetErrNo(DWORD val)
{
	g_lastErrNo.store(val, std::memory_order_relaxed);
}
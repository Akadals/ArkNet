#include <AkaNetCore/Module/Accept/AcceptExStrategy.h>
#include <AkaNetCore/Logger.h>
#include <AkaNetCore/Opt.h>
#include "../Internal.h"

#define DEFAULT_ACCEPT_RESERVATION_COUNT 100;

namespace
{
	UINT16 g_acceptResCount = DEFAULT_ACCEPT_RESERVATION_COUNT;

	LPFN_ACCEPTEX g_lpAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS g_lpGetAcceptExSockaddrs;
}
void AkaNetCore::Internal::Module::Accept::SetOptValue(UINT32 opt, const void* param)
{
	switch (opt)
	{
	case OPT_MODULE_ACCEPT_RESERVATION_COUNT:
	{
		g_acceptResCount = *static_cast<const UINT16*>(param);
		PRINT_DETAIL(std::string("OPT_MODULE_ACCEPT_RESERVATION_COUNT has been set to ") + std::to_string(g_acceptResCount));
		break;
	}
	}
}
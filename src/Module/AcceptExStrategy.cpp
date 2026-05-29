#include <AkaNetCore/Platform/Win32/Module/Accept/AcceptExStrategy.h>
#include <AkaNetCore/Platform/Win32/Logger.h>
#include <AkaNetCore/Platform/Win32/Opt.h>
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

void AkaNetCore::Module::Accept::LoadAcceptEx()
{
	DWORD bytes = 0;

	auto loadExtension = [&](GUID guid, void** fnPtr)
		{ if (WSAIoctl(listenSock, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guid, sizeof(guid), fnPtr, sizeof(*fnPtr),
			&bytes, NULL, NULL) == SOCKET_ERROR) PRINT_INFO("TestErr"); };

	loadExtension(WSAID_ACCEPTEX, reinterpret_cast<void**>(&g_lpAcceptEx));
	loadExtension(WSAID_GETACCEPTEXSOCKADDRS, reinterpret_cast<void**>(&g_lpGetAcceptExSockaddrs));
}

void AkaNetCore::Module::Accept::PostAccept()
{
	PIOCONTEXT ctx = new IOCONTEXT();

	if ((ctx->owner->sock = WSASocket(AF_INET, SOCK_STREAM,
		0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		PRINT_INFO("TestErr");
		delete ctx;
		return;
	}

	ctx->overlappedEx.ioType = IO_ACCEPT;

	DWORD bytes = 0;
	DWORD dwLen = sizeof(SOCKADDR_IN) + 16;

	BOOL ret = g_lpAcceptEx(listenSock, ctx->owner->sock, ctx->overlappedEx.buf,
		0, dwLen, dwLen, &bytes, &ctx->overlappedEx.overlapped);

	if (!ret) PRINT_INFO("TestErr");
}
#pragma once
#include <AkaNetCore/Core.h>
#include <WinSock2.h>
#include <MSWSock.h>

namespace AkaNetCore
{
	typedef class SocketAddressExpend SOCKADDR_EX, * PSOCKADDR_EX;

	class alignas(64) SocketAddressExpend
	{
	public:
		SOCKADDR_IN					sockAdr				= {};
		DWORD						sockAdrIP			= NULL;
		USHORT						sockAdrPort			= NULL;
	public:
		SocketAddressExpend() = default;
	};
}
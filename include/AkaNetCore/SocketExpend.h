#pragma once

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <sys/epoll.h>
#endif

namespace AkaNetCore
{
	typedef class SocketExpend			SOCKET_EX,		* PSOCKET_EX;
	typedef class SocketAddressExpend	SOCKADDR_EX,	* PSOCKADDR_EX;

	class alignas(64) SocketAddressExpend
	{
	public:
		SOCKADDR_IN					sockAdr = {};
		DWORD						sockAdrIP = NULL;
		USHORT						sockAdrPort = NULL;
	public:
		SocketAddressExpend() = default;
	};

	class SocketExpend
	{
	public:
#ifdef _WIN32
		SOCKET sock;
		SOCKADDR_EX sockAdrEx;
#else
		int sock;
		sockaddr_in sockAdr;
#endif
	public:
#ifdef _WIN32
		SocketExpend(SOCKET init);
#else
		SocketExpend(int init);
#endif
	};
}
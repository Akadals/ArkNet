#pragma once
#include <AkaNetCore/Platform/Win32/Core.h>
#include <AkaNetCore/Platform/Win32/IOContext.h>
#include <AkaNetCore/Platform/Win32/RingBuffer.h>
#include <AkaNetCore/Platform/Win32/Job.h>

#include <WinSock2.h>
#include <mutex>
#include <atomic>
#include <queue>

#define MAX_CTX_POOL_SIZE 0x8
#define MAX_JOB_POOL_SIZE 0x10

namespace AkaNetCore
{
	typedef class Session				SESSION, * PSESSION;
	typedef class SocketAddressExpend	SOCKADDR_EX, * PSOCKADDR_EX;

	class alignas(64) SocketAddressExpend
	{
	public:
		SOCKADDR_IN					sockAdr = {};
		DWORD						sockAdrIP = NULL;
		USHORT						sockAdrPort = NULL;
	public:
		SocketAddressExpend() = default;
	};

	class alignas(64) Session : public LockFreePoolAvailable
	{
	public:
		SOCKET sock = INVALID_SOCKET;
		SOCKADDR_EX sockAdrEx = {};

		DWORD timeoutTIme = NULL;

		RBUF recvBuf = { MAX_BUF_SIZE << 1 };
		RBUF sendBuf = { MAX_BUF_SIZE << 1 };

		WLFPOOL<IOCONTEXT> contextPool = { MAX_CTX_POOL_SIZE };

		WLFPOOL<JOB> jobPool{ MAX_JOB_POOL_SIZE };
		std::queue<PJOB> jobQueue = {};

		SRWLOCK sl = {};

		std::atomic<bool> alive = false;

		alignas(64) DWORD ioCount = 0x80000000;
		alignas(64) DWORD ioFlag = 0;
	public:
		Session();
		void Init() override;
		void Init(SOCKET sock);
	};
}
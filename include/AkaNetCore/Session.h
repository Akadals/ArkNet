#pragma once
#include <AkaNetCore/Core.h>
#include <AkaNetCore/IOContext.h>
#include <AkaNetCore/RingBuffer.h>
#include <AkaNetCore/Job.h>
#include <AkaNetCore/SocketExpend.h>

#include <WinSock2.h>
#include <mutex>
#include <atomic>
#include <queue>

#define MAX_CTX_POOL_SIZE 0x8
#define MAX_JOB_POOL_SIZE 0x10

namespace AkaNetCore
{
	typedef class Session				SESSION, * PSESSION;

	class alignas(64) Session : public LockFreePoolAvailable
	{
	private:
		SOCKET_EX sock = INVALID_SOCKET;

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
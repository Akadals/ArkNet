#pragma once
#include <AkaNetCore/Core.h>
#include <AkaNetCore/IOContext.h>
#include <AkaNetCore/RingBuffer.h>
#include <AkaNetCore/Job.h>
#include <AkaNetCore/Session.h>

#include <WinSock2.h>
#include <mutex>
#include <atomic>
#include <queue>


namespace AkaNetCore
{
	typedef class TLSSession				TLS_SESSION, * PTLS_SESSION;

	class alignas(64) TLSSession : public Session
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
		TLSSession();
		void Init() override;
		void Init(SOCKET sock);
	};
}
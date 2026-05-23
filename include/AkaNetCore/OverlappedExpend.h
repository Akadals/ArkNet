#pragma once
#include <AkaNetCore/Core.h>

#define IO_ACCEPT IOType::ACCEPT
#define IO_READING IOType::READING
#define IO_WRITING IOType::WRITING

namespace AkaNetCore
{
	enum class IOType : uint8_t
	{ ACCEPT, READING, WRITING };

	typedef class OverlappedExpend OVERLAPPED_EX, * POVERLAPPED_EX;

	class alignas(64) OverlappedExpend
	{
	public:
		OVERLAPPED		overlapped			= {};
		char			buf[MAX_BUF_SIZE]	= {};
		WSABUF			wsaBuf				= {};
		IOType			ioType				= IO_READING;
	public:
		OverlappedExpend() = default;
	};
}
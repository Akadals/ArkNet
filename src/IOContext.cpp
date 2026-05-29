#include <AkaNetCore/Platform/Win32/IOContext.h>
#include <AkaNetCore/Platform/Win32/Session.h>

AkaNetCore::IOContext::IOContext()
{
	Init();
}

void AkaNetCore::IOContext::Init()
{
	ZeroMemory(&overlappedEx.overlapped, sizeof(OVERLAPPED));
	ZeroMemory(&overlappedEx.buf, MAX_BUF_SIZE);

	overlappedEx.wsaBuf.len = 0;
	overlappedEx.wsaBuf.buf = nullptr;

	owner = nullptr;
}

void AkaNetCore::IOContext::Init(PSESSION owner, IOType ioType)
{
	ZeroMemory(&overlappedEx.overlapped, sizeof(OVERLAPPED));
	ZeroMemory(&overlappedEx.buf, MAX_BUF_SIZE);

	this->owner = owner;

	overlappedEx.ioType = ioType;

	overlappedEx.wsaBuf.buf = overlappedEx.buf;
	overlappedEx.wsaBuf.len = sizeof(overlappedEx.buf);
}

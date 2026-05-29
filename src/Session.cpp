#include <AkaNetCore/Platform/Win32/Session.h>

AkaNetCore::Session::Session()
{
	InitializeSRWLock(&sl);
	Init();
}

void AkaNetCore::Session::Init()
{
	sock = INVALID_SOCKET;
	sockAdrEx = {};
	timeoutTIme = NULL;
	alive = false;
	ioCount = 0x80000000;
	ioFlag = 0;
}

void AkaNetCore::Session::Init(SOCKET sock)
{
	this->sock = sock;
	alive = true;
}

#pragma once
#include <AkaNetCore/Platform/Win32/Core.h>
#include <AkaNetCore/Platform/Win32/IOContext.h>
#include <AkaNetCore/Platform/Win32/Session.h>

#ifndef INCL_ACCEPT_MODULE
#define INCL_ACCEPT_MODULE
#endif

namespace AkaNetCore
{
	namespace Module
	{
		namespace Accept
		{
			void LoadAcceptEx();
			void PostAccept();
		}
	}
}
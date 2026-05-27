#pragma once
#include <AkaNetCore/Core.h>
#include <AkaNetCore/IOContext.h>
#include <AkaNetCore/Session.h>

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
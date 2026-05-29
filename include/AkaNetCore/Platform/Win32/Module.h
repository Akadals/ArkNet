


#ifdef AKANETCORE_USE_ACCEPT_STRATEGY
#include <AkaNetCore/Module/Accept/AcceptStrategy.h>
#endif

#ifdef AKANETCORE_USE_ACCEPT_EX_STRATEGY
#ifdef INCL_ACCEPT_MODULE
#error Another Accept module has already been included.
#else
#include <AkaNetCore/Platform/Win32/Module/Accept/AcceptExStrategy.h>
#endif
#endif

#ifdef AKANETCORE_USE_ACCEPT_TLS_STRATEGY
#ifdef INCL_ACCEPT_MODULE
#error Another Accept module has already been included.
#else
#include <AkaNetCore/Module/Accept/AcceptTLSStrategy.h>
#endif
#endif

#ifdef AKANETCORE_USE_ACCEPT_SINGLE_STRATEGY
#ifdef INCL_ACCEPT_MODULE
#error Another Accept module has already been included.
#else
#include <AkaNetCore/Module/Accept/AcceptSingleStrategy.h>
#endif
#endif

#include <AkaNetCore/Platform/Win32/Opt.h>

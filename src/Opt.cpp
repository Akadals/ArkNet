#include <AkaNetCore/Opt.h>
#include <AkaNetCore/Logger.h>
#include "Internal.h"


void AkaNetCore::SetOpt(UINT32 opt, bool optval)
{
	switch (opt)
	{
	case OPT_LOGGER_ENABLE_COLORED:
	case OPT_LOGGER_ENABLE_FILE_OUTPUT:
		Internal::Logger::SetOptValue(opt, &optval);
		break;
	default:
		LOG_ERROR("SetOpt type mismatch or Unallowed bitflag option");
		LOG_DETAIL("The required type is std::filesystem::path");
		break;
	}
}

void AkaNetCore::SetOpt(UINT32 opt, std::string optval)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	case OPT_LOGGER_FILE_OUTPUT_PATH:
		Internal::Logger::SetOptValue(opt, &optval);
		break;
	default:
		LOG_ERROR("SetOpt type mismatch or Unallowed bitflag option");
		LOG_DETAIL("The required type is std::string"); 
		break;
	}
}

void AkaNetCore::SetOpt(UINT32 opt, const char* optval)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	case OPT_LOGGER_FILE_OUTPUT_PATH:
	{
		std::string str(optval);
		Internal::Logger::SetOptValue(opt, &str);
		break;
	}
	default:
		LOG_ERROR("SetOpt type mismatch or Unallowed bitflag option");
		LOG_DETAIL("The required type is const char*");
		break;
	}
}

void AkaNetCore::SetOpt(UINT32 opt, int optval)
{
	switch (opt)
	{
	case OPT_LOGGER_LOGGING_LEVEL:
		Internal::Logger::SetOptValue(opt, &optval);
		break;
	case OPT_MODULE_ACCEPT_RESERVATION_COUNT:
		//Internal::Module::Accept::SetOptValue(opt, &optval);
		break;
	default:
		LOG_ERROR("SetOpt type mismatch or Unallowed bitflag option");
		LOG_DETAIL("The required type is int");
		break;
	}
}
void AkaNetCore::SetOpt(UINT32 opt)
{
	switch (opt)
	{
	case OPT_LOGGER_START_WRITE:
		Internal::Logger::SetOptValue(opt, NULL);
		break;
	default:
		LOG_ERROR("SetOpt type mismatch or Unallowed bitflag option");
		break;
	}
}
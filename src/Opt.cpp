#include <AkaNetCore/Opt.h>
#include <AkaNetCore/Logger.h>
#include "Internal.h"

void AkaNetCore::SetOpt(uint8_t opt, bool param)
{
	switch (opt)
	{
	case OPT_LOGGER_ENABLE_FILE_OUTPUT: Internal::Logger::SetOptValue(opt, param); break;
	}
}

void AkaNetCore::SetOpt(uint8_t opt, std::filesystem::path param)
{
	switch (opt)
	{
	case OPT_LOGGER_FILE_OUTPUT_PATH: Internal::Logger::SetOptValue(opt, param); break;
	}
}

void AkaNetCore::SetOpt(uint8_t opt, std::string param)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT: Internal::Logger::SetOptValue(opt, param); break;
	}
}

void AkaNetCore::SetOpt(uint8_t opt, const char* param)
{
	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
		Internal::Logger::SetOptValue(opt, std::string(param));
		break;
	case OPT_LOGGER_FILE_OUTPUT_PATH:
		Internal::Logger::SetOptValue(opt, std::filesystem::path(param));
		break;
	}
}

void AkaNetCore::SetOpt(uint8_t opt, int param)
{
	switch (opt)
	{
	case OPT_LOGGER_LOGGING_LEVEL:
		Internal::Logger::SetOptValue(opt, param);

	}
}

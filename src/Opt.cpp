#include <AkaNetCore/Opt.h>

void AkaNetCore::SetOpt(uint8_t opt, const OptParam& param)
{
	auto Validate = [&]<typename T>(const char* optName)
	{
		if (!std::holds_alternative<T>(param))
		{
			PRINT_EXCAPTION(std::string("Type mismatch, the type of ") + optName + " must be " + typeid(T).name());
			return false;
		}
		return true;
	};

	switch (opt)
	{
	case OPT_LOGGER_TIME_FORMAT:
	{
		using T = std::decay_t<decltype(Logger::s_timeFormat)>;
		if (Validate.template operator() < T > ("OPT_LOGGER_TIME_FORMAT"))
			Logger::s_timeFormat = get<T>(param);
		break;
	}
	case OPT_LOGGER_ENABLE_FILE_OUTPUT:
	{
		if (Logger::s_worker) PRINT_WARNING("Cannot change OPT_LOGGER_ENABLE_FILE_OUTPUT at present");
		using T = std::decay_t<decltype(Logger::s_enableFileOutput)>;
		if (Validate.template operator() < T > ("OPT_LOGGER_ENABLE_FILE_OUTPUT"))
			Logger::s_enableFileOutput = get<T>(param);
		Logger::Startup();
		break;
	}
	case OPT_LOGGER_FILE_OUTPUT_PATH:
	{
		if (Logger::s_worker) PRINT_WARNING("Cannot change OPT_LOGGER_FILE_OUTPUT_PATH at present");
		using T = std::decay_t<decltype(Logger::s_OutputPath)>;
		if (Validate.template operator() < T > ("OPT_LOGGER_FILE_OUTPUT_PATH"))
			Logger::s_OutputPath = get<T>(param);
		break;
	}
	}
}

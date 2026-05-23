#include <AkaNetCore/Core.h>
#include <AkaNetCore/Logger.h>

#include <variant>
#include <type_traits>
#include <utility> 
#include <filesystem>
#include <string>

#define OPT_LOGGER_TIME_FORMAT 0
#define OPT_LOGGER_ENABLE_FILE_OUTPUT 1
#define OPT_LOGGER_FILE_OUTPUT_PATH 2

namespace AkaNetCore
{
	using OptParam = std::variant<std::string, bool, int, float, std::filesystem::path>;

	void SetOpt(uint8_t opt, const OptParam& param);
}
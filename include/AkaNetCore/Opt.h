#include <AkaNetCore/Core.h>
#include <filesystem>

#define OPT_LOGGER_TIME_FORMAT 0
#define OPT_LOGGER_ENABLE_FILE_OUTPUT 1
#define OPT_LOGGER_FILE_OUTPUT_PATH 2
#define OPT_LOGGER_LOGGING_LEVEL 3

/*
#======================Option Type Requirements======================#
|																	 |
|	* OPT_LOGGER_TIME_FORMAT			:	 std::string			 |
|	* OPT_LOGGER_ENABLE_FILE_OUTPUT		:	 bool					 |
|	* OPT_LOGGER_FILE_OUTPUT_PATH		:	 std::filesystem::path	 |
|	* OPT_LOGGER_LOGGING_LEVEL			:	 int					 |
|																	 |
#====================================================================#
*/

namespace AkaNetCore
{
	void SetOpt(uint8_t opt, bool param);
	void SetOpt(uint8_t opt, std::filesystem::path param);
	void SetOpt(uint8_t opt, std::string param);
	void SetOpt(uint8_t opt, const char* param);
	void SetOpt(uint8_t opt, int param);
}
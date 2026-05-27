#include <AkaNetCore/Core.h>
#include <filesystem>

#define OPT_LOGGER_ENABLE_COLORED				0
#define OPT_LOGGER_ENABLE_FILE_OUTPUT			1
#define OPT_LOGGER_TIME_FORMAT					2
#define OPT_LOGGER_FILE_OUTPUT_PATH				3
#define OPT_LOGGER_LOGGING_LEVEL				4

#define OPT_MODULE_ACCEPT_RESERVATION_COUNT		5

/*
#==========================Option Type Requirements=========================#
|																			|
|	* OPT_LOGGER_TIME_FORMAT					:	std::string				|
|	* OPT_LOGGER_ENABLE_FILE_OUTPUT				:	bool					|
|	* OPT_LOGGER_FILE_OUTPUT_PATH				:	std::filesystem::path	|
|	* OPT_LOGGER_LOGGING_LEVEL					:	UINT8					|
|	* OPT_LOGGER_ENABLE_COLORED					:	bool					|
|	* OPT_MODULE_ACCEPT_RESERVATION_COUNT		:	UINT16					|
|																			|
#===========================================================================#
*/

namespace AkaNetCore
{
	void SetOpt(UINT32 opt, bool optval);
	void SetOpt(UINT32 opt, std::filesystem::path optval);
	void SetOpt(UINT32 opt, std::string optval);
	void SetOpt(UINT32 opt, const char* optval);
	void SetOpt(UINT32 opt, int optval);
}
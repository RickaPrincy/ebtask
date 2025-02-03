#include "../utils/logger.hpp"
#include "os.hpp"

bool ebtask::os::execute_command(const std::string &command, bool log)
{
	if (command.empty())
	{
		if (log)
		{
			ebtask::warn("Empty command was received");
		}
		return true;
	}
	const char *cmd = command.c_str();
	const auto result_status = std::system(cmd);

	if (log)
		ebtask::log("EXECUTED: " + command + " -> [ RESPONSE_CODE : " + std::to_string(result_status) + "]");

	return result_status == 0;
}

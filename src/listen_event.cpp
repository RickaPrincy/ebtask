#include <iostream>

#include "command_actions.h"
#include "configuration/configuration.h"
#include "configuration/types.h"
#include "utils/logger.h"

ECallBack::ReadFunction listen_event()
{
	ELogger::log("Loading your ebtask keys config");
	load_keys();

	return [](int code, KeyStatus status, int &fd, const char *devnode)
	{
		std::cout << "code from listen_event" << code << std::endl;
		return false;
	};
}

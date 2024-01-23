#include <iostream>

#include "command_actions.h"
#include "configuration/configuration.h"
#include "configuration/types.h"
#include "utils/logger.h"

ECallBack::ReadFunction listen_event()
{
	ELogger::log("Loading your ebtask modes configuration");
	load_modes();
	ELogger::log("Loading your ebtask modes configuration DONE !!");
	ELogger::log("Loading your ebtask keys config");
	load_keys();
	ELogger::log("Loading your ebtask keys config DONE !!");
	ELogger::log("Ebtask is now listen to your event !!!");

	return [](int code, KeyStatus status, int &fd, const char *devnode)
	{
		if (update_key_status(code, status) == KeyStatus::NOT_FOUND)
			return false;

		if (handle_mode())
			return false;

		handle_action();

		return false;
	};
}

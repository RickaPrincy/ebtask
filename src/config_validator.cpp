#include <iostream>

#include "config.h"
#include "utils.h"

using json = nlohmann::json;

Config get_config_if_valid(nlohmann::json config)
{
	if (!config.is_object())
	{
		ebtask::exit_error("ebtask config must be an object");
	}

	if (!config["stop"].is_array())
	{
		ebtask::exit_error("Must set a keybind to stop ebtask (ex: [\"KEY_ENTER\", \"KEY_LEFTSHIFT\"])");
	}

	if (!config["modes"].is_array())
	{
		ebtask::exit_error("modes must be present and must be array");
	}

    return Config();
}

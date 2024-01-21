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

	if (!config["mods"].is_array())
	{
	}
}

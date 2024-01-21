#include "config.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "utils.h"

using json = nlohmann::json;

static const char* EBTASK_CONFIG_ENV = "EBTASK_PATH";
static char* EBTASK_CONFIG_VALUE = std::getenv(EBTASK_CONFIG_ENV);

Config get_config()
{
	if (EBTASK_CONFIG_VALUE == nullptr)
	{
		std::string config_path = EBTASK_CONFIG_ENV;
		ebtask::exit_error("Make sure that " + config_path + "env exist on your environment");
	}

	std::string ebtask_path_value = EBTASK_CONFIG_VALUE;

	std::ifstream ebtask_config(ebtask_path_value);
	if (!ebtask_config.is_open())
	{
		ebtask::exit_error("Cannot read " + ebtask_path_value);
	}

	json config;
	try
	{
		config = json::parse(ebtask_config);
		ebtask_config.close();
	}
	catch (json::exception error)
	{
		std::cerr << "[ ERROR ]: Your config file is not a valid json" << std::endl;
		ebtask_config.close();
		exit(EXIT_FAILURE);
	}

	return get_config_if_valid(config);
}

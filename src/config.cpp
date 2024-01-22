#include "config.h"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "utils/exception.h"

using json = nlohmann::json;

static const char* EBTASK_CONFIG_ENV = "EBTASK_PATH";
static char* EBTASK_CONFIG_VALUE = std::getenv(EBTASK_CONFIG_ENV);

std::string get_config_path()
{
	if (EBTASK_CONFIG_VALUE == nullptr)
	{
		std::string config_path = EBTASK_CONFIG_ENV;
		throw NotFoundConfigurationError();
	}

	std::string config_path = EBTASK_CONFIG_VALUE;
	return config_path;
}

Config get_config()
{
	std::string ebtask_path_value = get_config_path();
	std::ifstream ebtask_config(ebtask_path_value + "/" + "ebtask.config.json");

	if (!ebtask_config.is_open())
		throw NotFoundConfigurationError();

	json config;
	try
	{
		config = json::parse(ebtask_config);
		ebtask_config.close();
	}
	catch (json::exception error)
	{
		ebtask_config.close();
		throw InvalidConfigurationError();
	}

	return get_config_if_valid(config, ebtask_path_value);
}

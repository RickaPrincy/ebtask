#include "config.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

static const char* EBTASK_CONFIG_ENV = "EBTASK_PATH";
static char* EBTASK_CONFIG_VALUE = std::getenv(EBTASK_CONFIG_ENV);

Config get_config()
{
	if (EBTASK_CONFIG_VALUE == nullptr)
	{
		std::cerr << "[ ERROR ]: Make sure that " << EBTASK_CONFIG_ENV << " env exist on your environment" << std::endl;
		exit(EXIT_FAILURE);
	}
    std::string ebtask_path_value = EBTASK_CONFIG_VALUE;

    std::ifstream ebtask_config(ebtask_path_value);
	if (!ebtask_config.is_open()) {
		std::cerr << "[ ERROR ]: Cannot read " << ebtask_path_value << std::endl;
		exit(EXIT_FAILURE);
	}

    json config;
	try {
		config = json::parse(ebtask_config);
		ebtask_config.close();
	} catch (json::exception error) {
		std::cerr << "[ ERROR ]: Your config file is not a valid json" << std::endl;
		ebtask_config.close();
		exit(EXIT_FAILURE);
	}
    
    return get_config_if_valid(config);
}

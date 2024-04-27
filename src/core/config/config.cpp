#include "config.hpp"

#include <stdexcept>

#include "../ebtask_config.hpp"

static char *EBTASK_CONFIG_VALUE = std::getenv(EBTASK_PATH_ENV);
const std::string FILE_PATH_SEPARATOR = "/";  // as ebtask is only for unix system

std::string ebtask::get_config_path()
{
	if (EBTASK_CONFIG_VALUE == nullptr || EBTASK_CONFIG_VALUE == NULL)
		throw std::runtime_error("EBTASK_PATH environment variable not found");
	return std::string(EBTASK_CONFIG_VALUE);
}

std::string ebtask::get_config_file_path(std::string file_name)
{
	return ebtask::get_config_path() + FILE_PATH_SEPARATOR + file_name;
}

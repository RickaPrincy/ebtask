#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>

#include "../utils/logger.h"
#include "exception.h"
#include "json/json.hpp"
#include "types.h"

using json = nlohmann::json;

static char *EBTASK_CONFIG_VALUE = std::getenv(EBTASK_PATH_ENV);

std::string get_config_path()
{
	if (EBTASK_CONFIG_VALUE == nullptr)
		throw NotFoundConfigurationError();

	std::string config_path = EBTASK_CONFIG_VALUE;
	return config_path;
}

std::tuple<std::string, nlohmann::json> get_configuraton_content_with_path(std::string file_name,
	bool is_required)
{
	std::string config_path = get_config_path();
	std::string config_file_path = config_path + "/" + file_name;
	std::ifstream config_file(config_file_path);

	if (!config_file.is_open() && is_required)
		throw NotFoundConfigurationError(config_file_path + " was not found");

	json config_file_content = json::object();

	try
	{
		if (config_file.is_open())
			config_file_content = json::parse(config_file);
	}
	catch (const json::exception &error)
	{
		config_file.close();
		throw InvalidConfigurationError(file_name + " is invalid");
	}
	config_file.close();

	return std::make_tuple(config_file_path, config_file_content);
}

void load_keybinding(const nlohmann::json &json_objet, std::vector<int> &target, std::string key)
{
	for (auto keybinding : json_objet[key])
	{
		target.push_back(keybinding);
	}
}

void save_json_file(const nlohmann::json &json_content, std::string path)
{
	std::ofstream file(path);
	file << std::setw(2) << json_content << std::endl;
	file.close();
}

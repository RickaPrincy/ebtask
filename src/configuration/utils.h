#ifndef __EBTASK_CONF_UTILS__
#define __EBTASK_CONF_UTILS__

#include <string>
#include <tuple>
#include <vector>

#include "json/json.hpp"

static std::vector<std::string> split_command(std::string command, std::string delimiter)
{
	std::vector<std::string> res;
	int pos = 0;
	while (pos < command.size())
	{
		pos = command.find(delimiter);
		res.push_back(command.substr(0, pos));
		command.erase(0, pos + 3);
	}
	return res;
}

std::tuple<std::string, nlohmann::json> get_configuraton_content_with_path(std::string file_name,
	bool is_required = false);
std::string get_configuration_path();

void load_keybinding(const nlohmann::json &json_objet,
	std::vector<int> &target,
	std::string key = "keybinding");

void save_json_file(const nlohmann::json &json_content, std::string path);
#endif	// !__EBTASK_CONF_UTILS__

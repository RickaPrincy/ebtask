#ifndef __EBTASK_CONF_UTILS__
#define __EBTASK_CONF_UTILS__

#include <string>
#include <tuple>
#include <vector>

#include "json/json.hpp"

std::tuple<std::string, nlohmann::json> get_configuraton_content_with_path(
	std::string file_name,
	bool is_required = false);
std::string get_configuration_path();

void load_keybinding(const nlohmann::json &json_objet, std::vector<int> &target, std::string key ="keybinding");
#endif	// !__EBTASK_CONF_UTILS__

#ifndef __EBTASK_CONFIG__
#define __EBTASK_CONFIG__

#include <map>
#include <string>
#include <vector>

#include "json/json.hpp"

struct Config;

Config get_config_if_valid(nlohmann::json config, std::string config_path);
Config get_config();
std::string get_config_path();

class Action
{
public:
	std::vector<std::string> _keybinding{};
	std::string _function{}, _command{};
	Action(){};
};

class Mode
{
public:
	std::vector<Action> _actions{};
	std::vector<std::string> _keybinding{};
	std::string _name;
	Mode(){};
};

class Config
{
public:
	std::vector<Mode> _modes{};
	std::vector<std::string> _stop_keybinding{};
	std::string _path;
	Config(){};
};

#endif	// !__EBTASK_CONFIG__

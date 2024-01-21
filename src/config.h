#ifndef __EBTASK_CONFIG__
#define __EBTASK_CONFIG__

#include <map>
#include <string>
#include <vector>

#include "json/json.hpp"

struct Config;

Config get_config_if_valid(nlohmann::json config);
Config get_config();

enum class ActionType{
    FUNCTION,
    KEY
};

struct Action
{
	std::string _keybinding{}, _command{};
    ActionType _type{ActionType::FUNCTION};
};

struct Mode
{
	std::vector<Action> _actions{};
	std::string _keybinding{};
};

struct Config{
    std::map<std::string, Mode> _modes{};
    std::vector<std::string> stop_keybinding{};
};


#endif	// !__EBTASK_CONFIG__

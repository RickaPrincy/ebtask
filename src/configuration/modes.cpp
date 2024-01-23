#include <iostream>
#include <vector>

#include "../utils/logger.h"
#include "../utils/xutils.h"
#include "configuration.h"
#include "exception.h"
#include "json/json.hpp"
#include "types.h"
#include "utils.h"

using json = nlohmann::json;
static std::vector<Mode> _modes_{};
static const Mode *_current_mode_ = nullptr;

void load_modes()
{
	const auto &[config_path, config_content] =
		get_configuraton_content_with_path(EBTASK_CONF_NAME, true);

	try
	{
		Mode normal_mode{};
		normal_mode._name = "NORMAL";
		load_keybinding(config_content, normal_mode._keybinding, "normal_mode_keybinding");
		_modes_.push_back(normal_mode);

		for (const auto mode : config_content["modes"])
		{
			Mode _new_mode_{};
			_new_mode_._name = mode["name"];
			_new_mode_._type = mode["type"];
			load_keybinding(mode, _new_mode_._keybinding);

			for (auto action : mode["actions"])
			{
				Action _new_action_{};
				_new_action_._command = action["command"];
				_new_action_._command = action["command"];
				_new_action_._function = action["function"];
				load_keybinding(action, _new_action_._keybinding);
				_new_mode_._actions.push_back(_new_action_);
			}
			_modes_.push_back(_new_mode_);
		}
	}
	catch (json::exception error)
	{
		throw InvalidConfigurationError("Your modes in ebtask config is not valid");
	}
}

bool handle_mode()
{
	for (const auto &mode : _modes_)
	{
		if (is_all_pressed(mode._keybinding))
		{
			_current_mode_ = &mode;
			ELogger::log("Switch to " + mode._name + " mode");
			return true;
		}
	}
	return false;
}

void handle_action()
{
	if (_current_mode_ == nullptr)
		return;

	for (const auto &action : _current_mode_->_actions)
	{
		if (is_all_pressed(action._keybinding) && _current_mode_->_type == "xclip")
		{
			execute_and_copy(action._command);
			ELogger::log(action._command + " [ EXECUTED AND COPIED ]");
		}
	}
}

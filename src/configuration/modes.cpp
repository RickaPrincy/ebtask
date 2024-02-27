#include <algorithm>
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
static std::string _current_function_name_ = "";
static std::string _current_function_arg_ = "";
static std::string *_current_text_ = &_current_function_name_;
static const std::string ARG_NAME = "$arg";

// TODO: Refactor all of this code
static void execute_command_by_current_mode(const std::string &command)
{
	if (_current_mode_->_type == "xclip")
	{
		execute_and_copy(command);
		execute_and_type("echo \"C\"");
	}
	else if (_current_mode_->_type == "xdotool")
		execute_and_type(command);
	else
		execute_command(command);

	if (_current_mode_->_log)
		ELogger::log(command + " [ EXECUTED ]");
}

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
			_new_mode_._is_function = mode["is_function"];
			_new_mode_._render_command = mode["on_render"];
			_new_mode_._unmount_command = mode["on_unmount"];
			_new_mode_._log = mode["log"];

			load_keybinding(mode, _new_mode_._keybinding);

			for (auto action : mode["actions"])
			{
				Action _new_action_{};
				_new_action_._command = action["command"];

				if (_new_mode_._is_function)
					_new_action_._function = action["function"];
				else
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

void reset_handling()
{
	if (_current_mode_ != nullptr)
		execute_command(_current_mode_->_unmount_command);
	execute_command(_current_mode_->_unmount_command);
	_current_function_name_ = _current_function_arg_ = "";
	_current_text_ = &_current_function_name_;

	_current_mode_ = nullptr;
	ELogger::log("Back the NORMAL modes");
}

bool handle_mode()
{
	for (const auto &mode : _modes_)
	{
		if (is_all_pressed(mode._keybinding))
		{
			if (_current_mode_ != nullptr)
				execute_command(_current_mode_->_unmount_command);
			_current_mode_ = &mode;
			execute_command(_current_mode_->_render_command);
			ELogger::log("Switch to " + mode._name + " mode");
			return true;
		}
	}
	return false;
}

void handle_action(int code)
{
	if (_current_mode_ == nullptr)
		return;

	if (_current_mode_->_is_function)
	{
		handle_function(code);
		return;
	}

	for (const auto &action : _current_mode_->_actions)
	{
		if (!_current_mode_->_is_function && is_all_pressed(action._keybinding))
			execute_command_by_current_mode(action._command);
	}
}

void handle_function(int code)
{
	if (is_special())
		return;

	if (is_backspace(code))
	{
		if (!_current_text_->empty())
			_current_text_->pop_back();
		return;
	}
	else if (is_space(code))
	{
		*_current_text_ += " ";
		return;
	}

	std::string key_value = get_value(code);

	if (key_value == "(")
	{
		_current_text_ = &_current_function_arg_;
		return;
	}
	else if (key_value == ")")
	{
		do_backspace(_current_function_name_.size() + _current_function_arg_.size() + 2);
		auto action = std::find_if(_current_mode_->_actions.begin(),
			_current_mode_->_actions.end(),
			[&](const auto &_action) { return _action._function == _current_function_name_; });
		if (action == _current_mode_->_actions.end())
		{
			xtype_string("[ERROR] : Function not found");
			reset_handling();
			return;
		}

		std::string command_with_arg = action->_command;
		size_t found = command_with_arg.find(ARG_NAME);
		if (found != std::string::npos)
			command_with_arg.replace(found, ARG_NAME.length(), _current_function_arg_);

		execute_command_by_current_mode(command_with_arg);
		reset_handling();
		return;
	}
	else if (_current_text_->size() > TEXT_LENGTH_LIMIT)
	{
		do_backspace(_current_function_name_.size() + _current_function_arg_.size());
		xtype_string("[ ERROR ]: Too long arg or function name");
		reset_handling();
		return;
	}
	*_current_text_ += key_value;
}

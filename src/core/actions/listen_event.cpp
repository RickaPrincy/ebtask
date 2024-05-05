#include <json/json.hpp>

#include "../../utils/logger.hpp"
#include "../../utils/os.hpp"
#include "../../utils/xutils.hpp"
#include "../config/config.hpp"
#include "../os_inputs/os_inputs.hpp"
#include "actions.hpp"
#include "key_handler.hpp"

using json = nlohmann::json;
static ebtask::EbtaskConfig ebtask_config;
static const ebtask::Mode* _CURRENT_MODE_ = nullptr;
static std::string _CURRENT_FUNCTION_NAME_ = "";
static std::string _CURRENT_FUNCTION_ARG_ = "";
static std::string* _CURRENT_TEXT_ = &_CURRENT_FUNCTION_NAME_;
static const std::string ARG_NAME = "$arg";
static const int TEXT_LENGTH_LIMIT = 5000;

// /!\ TODO: refactor
static void execute_command_by_current_mode(const std::string& command)
{
	if (_CURRENT_MODE_->_output_reader == "@XCLIP")
	{
		ebtask::execute_and_copy(command);
		ebtask::execute_and_type("echo \"C\"");
	}
	else if (_CURRENT_MODE_->_output_reader == "@XDOTOOL")
		ebtask::execute_and_type(command);
	else
		ebtask::execute_command(command);

	if (_CURRENT_MODE_->_log_action)
		ebtask::log(command + " [ EXECUTED ]");
}

static void reset_handling()
{
	if (_CURRENT_MODE_ != nullptr)
		ebtask::execute_command(_CURRENT_MODE_->_on_stop);
	_CURRENT_FUNCTION_NAME_ = _CURRENT_FUNCTION_ARG_ = "";
	_CURRENT_TEXT_ = &_CURRENT_FUNCTION_NAME_;

	_CURRENT_MODE_ = nullptr;
	ebtask::log("Back the NORMAL modes");
}

static bool handle_mode()
{
	for (const auto& mode : ebtask_config._modes)
	{
		if (ebtask::is_all_pressed(mode._keybinding))
		{
			if (_CURRENT_MODE_ != nullptr)
				ebtask::execute_command(_CURRENT_MODE_->_on_stop);
			_CURRENT_MODE_ = &mode;
			return true;
		}
	}
	return false;
}

static void handle_function(int code)
{
	if (ebtask::is_special())
		return;

	if (ebtask::is_backspace(code))
	{
		if (!_CURRENT_TEXT_->empty())
			_CURRENT_TEXT_->pop_back();
		return;
	}
	else if (ebtask::is_space(code))
	{
		*_CURRENT_TEXT_ += " ";
		return;
	}

	std::string key_value = ebtask::get_key_text_value(code);

	if (key_value == "(")
	{
		_CURRENT_TEXT_ = &_CURRENT_FUNCTION_ARG_;
		return;
	}
	else if (key_value == ")")
	{
		ebtask::execute_command(_CURRENT_MODE_->_input_cleaner);
		ebtask::do_backspace(_CURRENT_FUNCTION_NAME_.size() + _CURRENT_FUNCTION_ARG_.size() + 2);
		auto action = std::find_if(_CURRENT_MODE_->_actions.begin(),
			_CURRENT_MODE_->_actions.end(),
			[&](const auto& _action) { return _action._function == _CURRENT_FUNCTION_NAME_; });
		if (action == _CURRENT_MODE_->_actions.end())
		{
			ebtask::xtype_string("[ERROR] : Function not found");
			reset_handling();
			return;
		}

		std::string command_with_arg = action->_command;
		size_t found = command_with_arg.find(ARG_NAME);
		if (found != std::string::npos)
			command_with_arg.replace(found, ARG_NAME.length(), _CURRENT_FUNCTION_ARG_);

		execute_command_by_current_mode(command_with_arg);
		reset_handling();
		return;
	}
	else if (_CURRENT_TEXT_->size() > TEXT_LENGTH_LIMIT)
	{
		ebtask::do_backspace(_CURRENT_FUNCTION_NAME_.size() + _CURRENT_FUNCTION_ARG_.size());
		ebtask::xtype_string("[ ERROR ]: Too long arg or function name");
		reset_handling();
		return;
	}
	*_CURRENT_TEXT_ += key_value;
}

static void handle_action(int code)
{
	if (_CURRENT_MODE_ == nullptr)
		return;

	if (_CURRENT_MODE_->_handler_type == ebtask::ActionHandler::FUNCTION)
	{
		handle_function(code);
		return;
	}

	for (const auto& action : _CURRENT_MODE_->_actions)
	{
		if (!(_CURRENT_MODE_->_handler_type == ebtask::ActionHandler::FUNCTION) &&
			ebtask::is_all_pressed(action._keybinding))
		{
			execute_command_by_current_mode(action._command);
		}
	}
}

ebtask::ReaderFunction ebtask::listen_event(std::string layout_name, std::string config_name)
{
	std::string layout_path = ebtask::get_config_file_path(layout_name);
	std::string config_path = ebtask::get_config_file_path(config_name);

	ebtask::log("Loading " + layout_path + "...");
	ebtask::load_layout(layout_path);
	ebtask::log("Loading " + config_path + "...");
	ebtask_config = ebtask::EbtaskConfig::from_config_file(config_path);

	return [&](int code, ebtask::KeyStatus status, int& fd, const char* devnode)
	{
		if (ebtask::update_key_status(code, status) == KeyStatus::NOT_FOUND)
			return false;

		if (handle_mode())
			return false;

		if (status != KeyStatus::PRESSED)
			return false;

		handle_action(code);
		return false;
	};
}

#include <json/json.hpp>

#include "../../utils/logger.hpp"
#include "../../utils/os.hpp"
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
static const int TEXT_LENGTH_LIMIT = 5000;

// /!\ TODO: refactor
static std::string execute_command_by_current_mode(const std::string& command)
{
	std::string command_value = ebtask_config.replace_all_alias(command);
	return ebtask::execute_command(command_value, _CURRENT_MODE_->log_action);
}

static void reset_handling()
{
	if (_CURRENT_MODE_ != nullptr)
		ebtask::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->on_stop));

	_CURRENT_FUNCTION_NAME_ = _CURRENT_FUNCTION_ARG_ = "";
	_CURRENT_TEXT_ = &_CURRENT_FUNCTION_NAME_;
	_CURRENT_MODE_ = nullptr;

	// reset alias value
	ebtask_config.specific_alias["@input"] = "";
	ebtask_config.specific_alias["@output"] = "";
	ebtask_config.specific_alias["@isize"] = "";
	ebtask_config.specific_alias["@osize"] = "";

	ebtask::log("Switch mode to -> NORMAL");
}

static bool handle_mode()
{
	if (ebtask::is_all_pressed(ebtask_config.normal_mode_keybinding))
	{
		reset_handling();
		return true;
	}

	for (const auto& mode : ebtask_config.modes)
	{
		if (!ebtask::is_all_pressed(mode.keybinding))
			continue;

		if (_CURRENT_MODE_ != nullptr)
			ebtask::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->on_stop));

		_CURRENT_MODE_ = &mode;
		ebtask::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->on_start));
		ebtask::log("Switch mode to -> " + _CURRENT_MODE_->name);
		return true;
	}

	return false;
}

static void handle_function(int code)
{
	if (ebtask::is_special())  // no action if it's a special key
		return;

	if (ebtask::is_backspace(code))
	{
		if (!_CURRENT_TEXT_->empty())
			_CURRENT_TEXT_->pop_back();
		return;
	}

	if (ebtask::is_space(code))
	{
		*_CURRENT_TEXT_ += " ";
		return;
	}

	std::string key_value = ebtask::get_key_text_value(code);

	// TODO: REFACTOR
	if (key_value == "(")
	{
		_CURRENT_TEXT_ = &_CURRENT_FUNCTION_ARG_;
		return;
	}

	if (key_value == ")")
	{
		// STEP0: set input alias value
		ebtask_config.specific_alias["@input"] = _CURRENT_FUNCTION_ARG_;
		ebtask_config.specific_alias["@isize"] = std::to_string(_CURRENT_FUNCTION_ARG_.size());

		// STEP1: clean input
		ebtask::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->input_cleaner));

		auto action = std::find_if(_CURRENT_MODE_->actions.begin(),
			_CURRENT_MODE_->actions.end(),
			[&](const auto& _action) { return _action.function == _CURRENT_FUNCTION_NAME_; });

		if (action == _CURRENT_MODE_->actions.end())
		{
			// TODO: maybe a new features
			ebtask::cerr("Function not found");
			reset_handling();
			return;
		}

		// STEP2: execute the action
		std::string output = execute_command_by_current_mode(action->command);

		// STEP3: set all alias for output
		ebtask_config.specific_alias["@output"] = output;
		ebtask_config.specific_alias["@osize"] = std::to_string(output.size());

		reset_handling();
		return;
	}

	if (_CURRENT_TEXT_->size() > TEXT_LENGTH_LIMIT)
	{
		// STEP0: set input alias value (duplicate for perf)
		ebtask_config.specific_alias["@input"] = _CURRENT_FUNCTION_ARG_;
		ebtask_config.specific_alias["@isize"] = std::to_string(_CURRENT_FUNCTION_ARG_.size());

		ebtask::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->input_cleaner));
		ebtask::cerr("[ ERROR ]: Too long input or function name");
		reset_handling();
		return;
	}

	*_CURRENT_TEXT_ += key_value;
}

static void handle_action(int code)
{
	if (_CURRENT_MODE_ == nullptr)
		return;

	if (_CURRENT_MODE_->handler_type == ebtask::ActionHandler::FUNCTION)
	{
		handle_function(code);
		return;
	}

	for (const auto& action : _CURRENT_MODE_->actions)
	{
		if (_CURRENT_MODE_->handler_type != ebtask::ActionHandler::FUNCTION ||
			ebtask::is_all_pressed(action.keybinding))
		{
			execute_command_by_current_mode(action.command);
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
	ebtask_config = ebtask::EbtaskConfig::get_config_from_config_file(config_path);

	return [&](int code, ebtask::KeyStatus status, int& fd, const char* devnode)
	{
		// TODO: allow repeated
		if (status == KeyStatus::REPEATED)
			return true;

		if (ebtask::update_key_status(code, status) == KeyStatus::NOT_FOUND)
			return true;

		if (handle_mode())
			return true;

		if (status != KeyStatus::PRESSED)
			return true;

		handle_action(code);
		return true;
	};
}

#include <nlohmann/json.hpp>

#include "../../os/keyhandler.hpp"
#include "../../os/os.hpp"
#include "../../utils/logger.hpp"
#include "../config/config.hpp"
#include "actions.hpp"

static ebtask::config::EbtaskConfig ebtask_config;
static const ebtask::config::Mode* _CURRENT_MODE_ = nullptr;
static const int INPUT_MAX_LENGTH = 6000;
static std::string _CURRENT_TYPED_FUNCTION_NAME_ = "";
static std::string _CURRENT_TYPED_FUNCTION_ARG_ = "";
static std::string* _CURRENT_VALUE_TO_TYPE_ = &_CURRENT_TYPED_FUNCTION_NAME_;

static void execute_command_by_current_mode(const std::string& command)
{
	std::string command_value = ebtask_config.replace_all_alias(command);
	ebtask::os::execute_command(command_value, _CURRENT_MODE_->log_action);
	return;
}

static void reset()
{
	if (_CURRENT_MODE_ != nullptr)
		ebtask::os::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->on_stop));

	// reset function value
	_CURRENT_TYPED_FUNCTION_NAME_ = _CURRENT_TYPED_FUNCTION_ARG_ = "";
	_CURRENT_VALUE_TO_TYPE_ = &_CURRENT_TYPED_FUNCTION_NAME_;
	_CURRENT_MODE_ = nullptr;

	// reset alias value
	ebtask_config.specific_alias["@input"] = "";
	ebtask::log("Switch mode to -> NORMAL");
}

static bool handle_mode()
{
	if (ebtask::keyhandler::is_all_pressed(ebtask_config.normal_mode_keybinding))
	{
		reset();
		return true;
	}

	for (const auto& mode : ebtask_config.modes)
	{
		if (!ebtask::keyhandler::is_all_pressed(mode.keybinding))
			continue;

		if (_CURRENT_MODE_ != nullptr)
			ebtask::os::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->on_stop));

		_CURRENT_MODE_ = &mode;
		ebtask::os::execute_command(ebtask_config.replace_all_alias(_CURRENT_MODE_->on_start));
		ebtask::log("Switch mode to -> " + _CURRENT_MODE_->name);
		return true;
	}

	return false;
}

static void handle_function(int code)
{
	if (ebtask::keyhandler::is_last_key_pressed_is_special())  // no action if it's a special key
		return;

	if (ebtask::keyhandler::is_backspace(code))
	{
		if (!_CURRENT_VALUE_TO_TYPE_->empty())
			_CURRENT_VALUE_TO_TYPE_->pop_back();
		return;
	}

	if (_CURRENT_VALUE_TO_TYPE_->size() > INPUT_MAX_LENGTH)
	{
		reset();
		ebtask::cerr("Too long text value");
		return;
	}

	if (ebtask::keyhandler::is_space(code))
	{
		*_CURRENT_VALUE_TO_TYPE_ += " ";
		return;
	}

	std::string key_value = ebtask::keyhandler::get_key_text_value(code);
	if (key_value == "(")
	{
		_CURRENT_VALUE_TO_TYPE_ = &_CURRENT_TYPED_FUNCTION_ARG_;
		return;
	}

	if (key_value == ")")
	{
		// STEP0: set input alias value
		ebtask_config.specific_alias["@input"] = _CURRENT_TYPED_FUNCTION_ARG_;

		auto action = std::find_if(_CURRENT_MODE_->actions.begin(),
			_CURRENT_MODE_->actions.end(),
			[&](const auto& _action) { return _action.function == _CURRENT_TYPED_FUNCTION_NAME_; });

		if (action == _CURRENT_MODE_->actions.end())
		{
			ebtask::cerr("Function not found");
			reset();
			return;
		}

		execute_command_by_current_mode(action->command);
		return;
	}
	*_CURRENT_VALUE_TO_TYPE_ += key_value;
}

static void handle_action(int code)
{
	if (_CURRENT_MODE_ == nullptr)
		return;

	if (_CURRENT_MODE_->handler_type == ebtask::config::ActionHandler::FUNCTION)
	{
		handle_function(code);
		return;
	}

	for (const auto& action : _CURRENT_MODE_->actions)
	{
		if (_CURRENT_MODE_->handler_type != ebtask::config::ActionHandler::FUNCTION ||
			ebtask::keyhandler::is_all_pressed(action.keybinding))
		{
			execute_command_by_current_mode(action.command);
		}
	}
}

ebtask::os::input::ReaderFunction ebtask::listen_event(std::string layout_name, std::string config_name)
{
	std::string layout_path = ebtask::config::EbtaskConfig::append_config_base_path(layout_name);
	std::string config_path = ebtask::config::EbtaskConfig::append_config_base_path(config_name);

	ebtask::log("Loading " + layout_path + "...");
	ebtask::keyhandler::load_layout_config(layout_path);
	ebtask::log("Loading " + config_path + "...");
	ebtask_config = ebtask::config::EbtaskConfig::read_ebtask_config_file(config_path);

	return [&](int code, ebtask::os::input::KeyStatus status, int& fd, const char* devnode)
	{
		// TODO: allow repeated
		if (status == ebtask::os::input::KeyStatus::REPEATED)
			return true;

		if (ebtask::keyhandler::update_key_status(code, status) == ebtask::os::input::KeyStatus::NOT_FOUND)
			return true;

		if (handle_mode())
			return true;

		if (status != ebtask::os::input::KeyStatus::PRESSED)
			return true;

		handle_action(code);
		return true;
	};
}

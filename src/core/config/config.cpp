#include "config.hpp"

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unordered_map>

#include "../../generated_config.hpp"
#include "../../utils/fs_utils.hpp"
#include "../../utils/json_utils.hpp"
#include "../../utils/logger.hpp"

namespace fs = std::filesystem;
const static char *EBTASK_CONFIG_ENV_VALUE = std::getenv(EBTASK_PATH_ENV_NAME);
const static std::unordered_map<std::string, ebtask::config::ActionHandler> handler_enum = {
	{ "FUNCTION", ebtask::config::ActionHandler::FUNCTION },
	{ "KEY_BINDING", ebtask::config::ActionHandler::FUNCTION }
};

static ebtask::config::ActionHandler get_action_handler_type(std::string handler)
{
	auto value = handler_enum.find(handler);
	if (value == handler_enum.end())
		throw std::runtime_error("Action handler type not valid (enum: [ FUNCTION | KEY_BINDING ])");
	return value->second;
}

static std::string replace_all_occurrence(const std::string &source,
	const std::string &to_replace,
	const std::string &replace_value)
{
	std::string result = source;
	size_t position = result.find(to_replace);

	while (position != std::string::npos)
	{
		result.replace(position, to_replace.length(), replace_value);
		position = result.find(to_replace, position + replace_value.length());
	}

	return result;
}

std::string ebtask::config::EbtaskConfig::get_config_base_path()
{
	if (EBTASK_CONFIG_ENV_VALUE == nullptr || EBTASK_CONFIG_ENV_VALUE == NULL)
		throw std::runtime_error("EBTASK_PATH environment variable not found");
	return std::string(EBTASK_CONFIG_ENV_VALUE);
}

std::string ebtask::config::EbtaskConfig::append_config_base_path(std::string file_name)
{
	return ebtask::config::EbtaskConfig::get_config_base_path() + ebtask::FILE_SEPARTOR + file_name;
}

std::string ebtask::config::EbtaskConfig::handle_config_file_already_exist_error(std::string file_name,
	std::string error_action)
{
	std::string file_path = ebtask::config::EbtaskConfig::append_config_base_path(file_name);
	if (!fs::exists(file_path))
		return file_name;

	auto action_on_error = ebtask::get_path_error_action_from_string(error_action.empty() ? "ERROR" : error_action);

	if (fs::exists(file_path) && action_on_error == ebtask::PathExistErrorAction::ERROR)
		throw std::runtime_error(file_path + " already exists, use --error-action to specify what to do");

	if (!fs::is_regular_file(file_path))
		throw std::runtime_error(file_path + " is not a valid layout config file");

	// switch not working on enum class value
	if (action_on_error == ebtask::PathExistErrorAction::OVERRIDE)
		ebtask::warn(file_path + " will be overridden");
	else
	{
		file_name = ebtask::get_dumb_unique_prefix() + file_name;
		ebtask::copy(file_path, ebtask::config::EbtaskConfig::append_config_base_path(file_name));
	}

	return file_name;
}

ebtask::config::EbtaskConfig ebtask::config::EbtaskConfig::generate_new_config_template()
{
	ebtask::config::EbtaskConfig config(true);
	ebtask::config::Action say_hello_action;
	say_hello_action.function = "sayHello";
	say_hello_action.command = "echo \"Hello @input\"";

	ebtask::config::Mode example_mode;
	example_mode.actions.push_back(say_hello_action);
	example_mode.handler_type = ebtask::config::ActionHandler::FUNCTION;
	example_mode.keybinding = { 42, 54 };
	example_mode.name = "example";

	config.normal_mode_keybinding = { 1, 42 };
	config.alias = { { "@wtype_erase", "" },
		{ "@wl_clipboard_copy", "wl-copy \"output\"" },
		{ "@wtype_type", "wtype \"@output\"" },
		{ "@xdotool_type", "xdotool type \"@output\"" },
		{ "@xclip_copy", "echo \"@output\" | xclip -selection clipboard" },
		{ "@xdotool_erase", "xdotool key --clearmodifiers --repeat @isize BackSpace" } };

	config.modes.push_back(example_mode);
	return config;
}

ebtask::config::EbtaskConfig ebtask::config::EbtaskConfig::read_ebtask_config_file(std::string file_config_path)
{
	ebtask::config::EbtaskConfig config;
	nlohmann::json file_content = ebtask::read_json_file(file_config_path, true);

	try
	{
		READ_KEYBIND(file_content, config, normal_mode_keybinding);
		for (const auto &alias : file_content["alias"])
			config.alias.insert({ alias["name"].get<std::string>(), alias["value"].get<std::string>() });

		for (const auto &mode : file_content["modes"])
		{
			ebtask::config::Mode new_mode;
			READ_JSON_DATA(mode, new_mode, name);
			READ_KEYBIND(mode, new_mode, keybinding);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, on_stop);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, on_start);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, log_action);
			new_mode.handler_type = get_action_handler_type(mode["handler_type"]);

			for (const auto &action : mode["actions"])
			{
				ebtask::config::Action new_action;
				READ_JSON_DATA(action, new_action, command);

				if (new_mode.handler_type == ebtask::config::ActionHandler::FUNCTION)
					READ_JSON_DATA(action, new_action, function);
				else
					READ_KEYBIND(action, new_action, keybinding);
				new_mode.actions.push_back(new_action);
			}
			config.modes.push_back(new_mode);
		}
	}
	catch (const nlohmann::json::exception &error)
	{
		ebtask::cerr("Your config file is not valid, please read the docs");
	}

	return config;
}

ebtask::config::EbtaskConfig &ebtask::config::EbtaskConfig::operator=(const ebtask::config::EbtaskConfig &other)
{
	if (this == &other)
		return *this;

	normal_mode_keybinding = other.normal_mode_keybinding;
	modes = other.modes;
	alias = other.alias;
	specific_alias = other.specific_alias;

	return *this;
}

ebtask::config::EbtaskConfig::EbtaskConfig(std::string file_config_path)
{
	*this = ebtask::config::EbtaskConfig::read_ebtask_config_file(file_config_path);
}

nlohmann::json ebtask::config::EbtaskConfig::to_json()
{
	nlohmann::json config_json = nlohmann::json::object();
	nlohmann::json modes_json = nlohmann::json::array();
	nlohmann::json alias = nlohmann::json::array();

	for (const auto &[name, value] : this->alias)
		alias.push_back({ { "name", name }, { "value", value } });

	for (const auto &mode : this->modes)
	{
		nlohmann::json new_mode;
		AFFECT_DATA_TO_JSON(new_mode, mode, name);
		new_mode["handler_type"] =
			mode.handler_type == ebtask::config::ActionHandler::FUNCTION ? "FUNCTION" : "KEY_BINDING";
		AFFECT_DATA_TO_JSON(new_mode, mode, log_action);
		AFFECT_DATA_TO_JSON(new_mode, mode, keybinding);
		AFFECT_DATA_TO_JSON(new_mode, mode, on_stop);
		AFFECT_DATA_TO_JSON(new_mode, mode, on_start);

		for (const auto &action : mode.actions)
		{
			nlohmann::json new_action;
			AFFECT_DATA_TO_JSON(new_action, action, command);
			if (mode.handler_type == ebtask::config::ActionHandler::FUNCTION)
				AFFECT_DATA_TO_JSON(new_action, action, function);
			else
				AFFECT_DATA_TO_JSON(new_action, action, keybinding);
			new_mode["actions"].push_back(new_action);
		}
		modes_json.push_back(new_mode);
	}

	config_json["alias"] = alias;
	config_json["modes"] = modes_json;
	AFFECT_DATA_TO_JSON(config_json, *this, normal_mode_keybinding);
	return config_json;
}

void ebtask::config::EbtaskConfig::save(std::string path)
{
	auto config_json = this->to_json();
	ebtask::write_json_file(path, config_json);
}

std::string ebtask::config::EbtaskConfig::replace_all_alias(std::string command)
{
	for (const auto &[name, value] : this->alias)
		command = replace_all_occurrence(command, name, value);

	for (const auto &[name, value] : this->specific_alias)
		command = replace_all_occurrence(command, name, value);

	return command;
}

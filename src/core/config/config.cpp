#include "config.hpp"

#include <json/json.hpp>
#include <stdexcept>
#include <unordered_map>

#include "../../utils/fs_utils.hpp"
#include "../../utils/json_utils.hpp"
#include "../../utils/logger.hpp"
#include "../ebtask_config.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

const static char *EBTASK_CONFIG_VALUE = std::getenv(EBTASK_PATH_ENV);
const static std::unordered_map<std::string, ebtask::ActionHandler> enum_hanlder = {
	{ "FUNCTION", ebtask::ActionHandler::FUNCTION },
	{ "KEY_BINDING", ebtask::ActionHandler::FUNCTION }
};

static ebtask::ActionHandler get_action_handler_type(std::string handler)
{
	auto value = enum_hanlder.find(handler);
	if (value == enum_hanlder.end())
		throw std::runtime_error(
			"Action handler type not valid (enum: [ FUNCTION | KEY_BINDING ])");
	return value->second;
}

std::string ebtask::get_config_path()
{
	if (EBTASK_CONFIG_VALUE == nullptr || EBTASK_CONFIG_VALUE == NULL)
		throw std::runtime_error("EBTASK_PATH environment variable not found");
	return std::string(EBTASK_CONFIG_VALUE);
}

std::string ebtask::get_config_file_path(std::string file_name)
{
	return ebtask::get_config_path() + ebtask::FILE_SEPARTOR + file_name;
}

ebtask::EbtaskConfig ebtask::EbtaskConfig::generate_new_config_template()
{
	ebtask::EbtaskConfig config;
	ebtask::Action say_hello_action;
	say_hello_action.function = "sayHello";
	say_hello_action.command = "echo \"Hello @input\"";

	ebtask::Mode example_mode;
	example_mode.actions.push_back(say_hello_action);
	example_mode.handler_type = ebtask::ActionHandler::FUNCTION;
	example_mode.keybinding = { 42, 54 };
	example_mode.output_reader = "@xdotool_type";
	example_mode.input_cleaner = "@xdotool_erase";
	example_mode.name = "example";

	config.normal_mode_keybinding = { 1, 42 };
	config.alias = { { "@xdotool_type", "xdotool type \"@output\"" },
		{ "@xclip_copy", "echo \"@output\" | xclip -selection clipboard" },
		{ "@xdotool_erase", "xdotool key --clearmodifiers --repeat @isize BackSpace" } };

	config.modes.push_back(example_mode);
	return config;
}

void ebtask::EbtaskConfig::save_config(std::string config_file_path)
{
	json config_json = json::object();
	json modes_json = json::array();
	json alias = json::array();

	for (const auto &[name, value] : this->alias)
		alias.push_back({ { "name", name }, { "value", value } });

	for (const auto &mode : this->modes)
	{
		json new_mode;
		AFFECT_DATA_TO_JSON(new_mode, mode, name);
		new_mode["handler_type"] =
			mode.handler_type == ebtask::ActionHandler::FUNCTION ? "FUNCTION" : "KEY_BINDING";
		AFFECT_DATA_TO_JSON(new_mode, mode, log_action);
		AFFECT_DATA_TO_JSON(new_mode, mode, keybinding);
		AFFECT_DATA_TO_JSON(new_mode, mode, input_cleaner);
		AFFECT_DATA_TO_JSON(new_mode, mode, output_reader);
		AFFECT_DATA_TO_JSON(new_mode, mode, on_stop);
		AFFECT_DATA_TO_JSON(new_mode, mode, on_start);

		for (const auto &action : mode.actions)
		{
			json new_action;
			AFFECT_DATA_TO_JSON(new_action, action, command);
			if (mode.handler_type == ebtask::ActionHandler::FUNCTION)
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
	ebtask::save_json_file(config_file_path, config_json);
}

ebtask::EbtaskConfig ebtask::EbtaskConfig::get_config_from_config_file(std::string file_config_path)
{
	ebtask::EbtaskConfig config;
	nlohmann::json file_content = ebtask::get_json_file_content(file_config_path, true);

	try
	{
		READ_KEYBIND(file_content, config, normal_mode_keybinding);
		for (const auto &alias : file_content["alias"])
			config.alias.insert(
				{ alias["name"].get<std::string>(), alias["value"].get<std::string>() });

		for (const auto &mode : file_content["modes"])
		{
			ebtask::Mode new_mode;
			READ_JSON_DATA(mode, new_mode, name);
			READ_KEYBIND(mode, new_mode, keybinding);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, on_stop);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, on_start);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, log_action);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, output_reader);
			READ_OPTIONAL_JSON_DATA(mode, new_mode, input_cleaner);
			new_mode.handler_type = get_action_handler_type(mode["handler_type"]);

			for (const auto &action : mode["actions"])
			{
				ebtask::Action new_action;
				READ_JSON_DATA(action, new_action, command);

				if (new_mode.handler_type == ebtask::ActionHandler::FUNCTION)
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

std::string ebtask::handle_config_file_already_exist_error(std::string file_name,
	std::string error_action)
{
	std::string file_path = ebtask::get_config_file_path(file_name);
	if (!fs::exists(file_path))
		return file_name;

	auto action_on_error =
		ebtask::get_path_error_action_from_string(error_action.empty() ? "ERROR" : error_action);

	if (fs::exists(file_path) && action_on_error == ebtask::PathExistErrorAction::ERROR)
		throw std::runtime_error(
			file_path + " already exists, use --error-action to specify what to do");

	if (!fs::is_regular_file(file_path))
		throw std::runtime_error(file_path + " is not a valid layout config file");

	// switch not working on enum class value
	if (action_on_error == ebtask::PathExistErrorAction::OVERRIDE)
		ebtask::warn(file_path + " will be overridden");
	else
	{
		file_name = ebtask::get_dumb_unique_prefix() + file_name;
		ebtask::copy_folder(file_path, ebtask::get_config_file_path(file_name));
	}

	return file_name;
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

std::string ebtask::EbtaskConfig::replace_all_alias(std::string command)
{
	for (const auto &[name, value] : this->alias)
		command = replace_all_occurrence(command, name, value);

	for (const auto &[name, value] : this->specific_alias)
		command = replace_all_occurrence(command, name, value);

	return command;
}

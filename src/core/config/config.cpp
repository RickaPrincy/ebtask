#include "config.hpp"

#include <json/json.hpp>
#include <stdexcept>
#include <unordered_map>

#include "../../utils/fs_utils.hpp"
#include "../../utils/logger.hpp"
#include "../ebtask_config.hpp"

const static char *EBTASK_CONFIG_VALUE = std::getenv(EBTASK_PATH_ENV);
const static std::string FILE_PATH_SEPARATOR = "/";	 // as ebtask is only for unix system
const static std::unordered_map<std::string, ebtask::ActionHandler> enum_hanlder = {
	{ "FUNCTION", ebtask::ActionHandler::FUNCTION },
	{ "KEY_BINDING", ebtask::ActionHandler::FUNCTION }
};

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string ebtask::get_config_path()
{
	if (EBTASK_CONFIG_VALUE == nullptr || EBTASK_CONFIG_VALUE == NULL)
		throw std::runtime_error("EBTASK_PATH environment variable not found");
	return std::string(EBTASK_CONFIG_VALUE);
}

std::string ebtask::get_config_file_path(std::string file_name)
{
	return ebtask::get_config_path() + FILE_PATH_SEPARATOR + file_name;
}

static void load_keybinding(const json &json_objet, ebtask::KeyBinding &target, std::string key)
{
	for (auto keybinding : json_objet[key])
	{
		target.push_back(keybinding);
	}
}

static ebtask::ActionHandler get_action_handler_type(std::string handler)
{
	auto value = enum_hanlder.find(handler);
	if (value == enum_hanlder.end())
		throw std::runtime_error(
			"Action handler type not valid (enum: [ FUNCTION | KEY_BINDING ])");
	return value->second;
}

void ebtask::EbtaskConfig::save_config(std::string config_file_path)
{
	json config_json = json::object();
	json modes_json = json::array();
	config_json["normal_mode_keybinding"] = this->_normal_mode_keybinding;
	for (const auto &mode : this->_modes)
	{
		json new_mode;
		new_mode["name"] = mode._name;
		new_mode["hanlder_type"] =
			mode._hanlder_type == ebtask::ActionHandler::FUNCTION ? "FUNCTION" : "KEY_BINDING";
		new_mode["log_action"] = mode._log_action;
		new_mode["keybinding"] = mode._keybinding;
		new_mode["input_cleaner"] = mode._input_cleaner;

		if (!mode._on_stop.empty())
			new_mode["on_stop"] = mode._on_stop;
		if (!mode._on_start.empty())
			new_mode["on_start"] = mode._on_start;
		if (!mode._output_reader.empty())
			new_mode["output_reader"] = mode._output_reader;

		for (const auto &action : mode._actions)
		{
			json new_action;
			new_action["command"] = action._command;
			new_action["output_reader"] = action._output_reader;
			if (mode._hanlder_type == ebtask::ActionHandler::FUNCTION)
				new_action["function"] = action._function;
			else
				new_action["keybinding"] = action._keybinding;
			new_mode["actions"].push_back(new_action);
		}
		modes_json.push_back(new_mode);
	}

	config_json["modes"] = modes_json;
	ebtask::save_json_file(config_file_path, config_json);
}

ebtask::EbtaskConfig ebtask::EbtaskConfig::generate_config_template()
{
	ebtask::EbtaskConfig config;
	ebtask::Action say_hello_action;
	say_hello_action._function = "sayHello";
	say_hello_action._command = "echo \"Hello $input\"";
	say_hello_action._output_reader = "@XDOTOOL";

	ebtask::Mode example_mode;
	example_mode._actions.push_back(say_hello_action);
	example_mode._hanlder_type = ebtask::ActionHandler::FUNCTION;
	example_mode._keybinding = { 42, 54 };
	example_mode._input_cleaner = "@XDOTOOL";
	example_mode._name = "example";

	config._normal_mode_keybinding = { 1, 42 };
	config._modes.push_back(example_mode);
	return config;
}

ebtask::EbtaskConfig ebtask::EbtaskConfig::from_config_file(std::string file_config_path)
{
	ebtask::EbtaskConfig config;
	nlohmann::json file_content = ebtask::get_json_file_content(file_config_path, true);

	try
	{
		load_keybinding(file_content, config._normal_mode_keybinding, "normal_mode_keybinding");
		for (const auto &mode : file_content["modes"])
		{
			ebtask::Mode new_mode;
			new_mode._name = mode["name"];
			new_mode._hanlder_type = get_action_handler_type(mode["handler_type"]);
			load_keybinding(mode, new_mode._keybinding, "keybinding");

			if (!mode["on_stop"].is_null())
				new_mode._on_stop = mode["on_stop"];
			if (!mode["on_start"].is_null())
				new_mode._on_stop = mode["on_start"];
			if (!mode["log_action"].is_null())
				new_mode._log_action = mode["log_action"];
			if (!mode["output_reader"].is_null())
				new_mode._output_reader = mode["output_reader"];

			for (const auto &action : mode["actions"])
			{
				ebtask::Action new_action;
				new_action._command = action["command"];

				if (action["output_reader"].is_null())
					new_action._output_reader = action["output_reader"];
				if (new_action._output_reader.empty())
					new_action._output_reader = new_mode._output_reader;

				if (new_mode._hanlder_type == ebtask::ActionHandler::FUNCTION)
					new_action._function = action["function"];
				else
					load_keybinding(action, new_action._keybinding, "keybinding");
				new_mode._actions.push_back(new_action);
			}
			config._modes.push_back(new_mode);
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
	if (error_action.empty())
		error_action = "ERROR";

	auto enum_error_action = ebtask::get_enum_string_path_error();
	auto error = enum_error_action.find(error_action);
	std::string file_path = ebtask::get_config_file_path(file_name);

	if (!fs::exists(file_path))
		return file_name;

	if (error == enum_error_action.end())
		throw std::runtime_error("Valid error_action are [ OVERRIDE | COPY | ERROR ]");

	if (fs::exists(file_path) && error->second == ebtask::PathExistErrorAction::ERROR)
		throw std::runtime_error(
			file_path + " already exists, use --error-action to specify what to do");

	if (!fs::is_regular_file(file_path))
		throw std::runtime_error(file_path + " is not a valid layout config file");

	// switch not working on enum class value
	if (error->second == ebtask::PathExistErrorAction::OVERRIDE)
		ebtask::warn(file_path + " will be overridden");
	else
	{
		file_name = ebtask::get_dumb_unique_prefix() + file_name;
		ebtask::copy_folder(file_path, ebtask::get_config_file_path(file_name));
	}

	return file_name;
}

#include <fcntl.h>
#include <unistd.h>

#include <limits>
#include <nlohmann/json.hpp>
#include <rcli/rcli.hpp>
#include <string>
#include <vector>

#include "../../utils/fs_utils.hpp"
#include "../../utils/logger.hpp"
#include "../config/config.hpp"
#include "actions.hpp"

static rcli::InputConfig input_config = rcli::InputConfig().clean(false).required(false);
static int special_recorded = 0;
static std::string current_config_path{};

static const std::array<std::string, 7>
	_SPECIAL_KEYS_NAMES_{ "ENTER", "CAPSLOCK", "LEFT_SHIFT", "RIGHT_SHIFT", "ALTGR", "BACKSPACE", "SPACE" };
using json = nlohmann::json;

static void save_key(ebtask::os::input::Key new_key)
{
	auto config_content = ebtask::read_json_file(current_config_path);
	try
	{
		if (!config_content.is_object())
			throw std::runtime_error("Ebtask layout config must be an object");

		if (!config_content.contains("keys"))
			config_content["keys"] = json::array();

		if (!config_content["keys"].is_array())
			throw std::runtime_error("Ebtask layout config \"keys\" must be an array");

		json new_json_key = json::object({ { "code", new_key.code },
			{ "normal", new_key.normal },
			{ "altgr", new_key.altgr },
			{ "capslock", new_key.capslock },
			{ "shift", new_key.shift } });
		config_content["keys"].push_back(new_json_key);
		ebtask::write_json_file(current_config_path, config_content);
	}
	catch (json::exception error)
	{
		throw std::runtime_error("Your current layout config is invalid");
	}
	catch (std::exception error)
	{
		// TODO: when we cannot no longer open the file or creating the file
		throw std::runtime_error("Cannot reopen the layout config file");
	}
}

static std::string ask_key_value(std::string special_key)
{
	return rcli::ask_input_value(input_config.text("Enter the value when \"" + special_key + "\" is PRESSED"));
}

static ebtask::os::input::Key ask_and_get_key_from_code(int code)
{
	ebtask::log("Keycode: " + std::to_string(code));
	ebtask::os::input::Key new_key{};
	new_key.normal = ask_key_value("NORMAL (nothing special)");

	if (new_key.normal.empty())
		return new_key;
	new_key.capslock = ask_key_value("CAPSLOCK");
	new_key.shift = ask_key_value("SHIFT (left or right)");
	new_key.altgr = ask_key_value("ALTGR");
	new_key.code = code;
	return new_key;
}

static ebtask::os::input::Key get_key_from_special_code(int code, std::string name)
{
	ebtask::log("Keycode: " + std::to_string(code));
	ebtask::os::input::Key special;
	special.code = code;
	special.normal = special.altgr = special.shift = special.capslock = name;
	return special;
}

// To retrieve one by one user's layout
ebtask::os::input::ReaderFunction ebtask::configure_layout(std::string layout_filename, std::string error_action)
{
	layout_filename =
		ebtask::config::EbtaskConfig::handle_config_file_already_exist_error(layout_filename, error_action);
	std::string config_path = ebtask::config::EbtaskConfig::append_config_base_path(layout_filename);
	current_config_path = config_path;
	std::vector<ebtask::os::input::Key> keys;

	ebtask::log("Layout file: " + config_path);
	ebtask::log("Type " + _SPECIAL_KEYS_NAMES_.at(special_recorded));

	return [&](int code, ebtask::os::input::KeyStatus status, int& fd, const char* devnode)
	{
		if (status != ebtask::os::input::KeyStatus::PRESSED)
			return true;

		ebtask::os::input::Key key;
		if (special_recorded >= _SPECIAL_KEYS_NAMES_.size())
		{
			key = ask_and_get_key_from_code(code);
			ebtask::log("Type any other key to record");
		}
		else
		{
			key = get_key_from_special_code(code, _SPECIAL_KEYS_NAMES_.at(special_recorded));
			special_recorded++;

			if (special_recorded < _SPECIAL_KEYS_NAMES_.size())
				ebtask::log("Type " + _SPECIAL_KEYS_NAMES_.at(special_recorded));
			else
			{
				ebtask::log("Now type any keys to record");
				ebtask::warn("Avoid typing the same key twice");
				ebtask::warn("To stop recording, just leave the NORMAL value empty");
			}
		}

		save_key(key);

		if (special_recorded == 1)
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// cleanup
		close(fd);
		fd = open(devnode, O_RDONLY);
		if (fd == -1)
			throw std::runtime_error("Cannot reopen devnode");
		return !key.normal.empty();
	};
}

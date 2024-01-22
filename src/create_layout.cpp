#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>

#include "command_actions.h"
#include "config.h"
#include "layout.h"
#include "utils/exception.h"
#include "utils/utils.h"

using json = nlohmann::json;
static std::unordered_map<int, Key> _keys_;

std::string get_line_value(std::string event)
{
	std::cout << "[ LOG ]: Value when " + event + " is pressed =>";
	std::string value;
	std::getline(std::cin, value);

	return value;
}

std::tuple<std::string, nlohmann::json> get_key_config()
{
	std::string config_path = get_config_path();
	std::string keys_path = config_path + "/" + "ebtask.keys.json";
	std::ifstream current_keys(keys_path);

	json current_keys_json = json::object();

	if (current_keys.is_open())
	{
		try
		{
			current_keys_json = json::parse(current_keys);
		}
		catch (const std::exception& e)
		{
			current_keys.close();
			throw InvalidKeyError();
		}
		current_keys.close();
	}

	if (!current_keys_json.contains("keys"))
		current_keys_json["keys"] = json::array();

	return std::make_tuple(keys_path, current_keys_json);
}

// save a key object in key config file
void save_key(const Key& key)
{
	auto [keys_path, current_keys_json] = get_key_config();

	try
	{
		json new_key = json::object();
		new_key["code"] = key._code;
		new_key["normal"] = key._normal;
		new_key["alt"] = key._alt;
		new_key["altgr"] = key._altgr;
		new_key["shift"] = key._shift;
		new_key["capslock"] = key._capslock;
		current_keys_json["keys"].push_back(new_key);
	}
	catch (json::exception error)
	{
		throw InvalidKeyError();
	}

	std::ofstream current_keys_output(keys_path);
	if (!current_keys_output.is_open())
		throw NotFoundConfigurationError();

	try
	{
		current_keys_output << std::setw(4) << current_keys_json;
		current_keys_output.close();
	}
	catch (const std::exception& e)
	{
		current_keys_output.close();
		throw NotFoundConfigurationError();
	}
}

// To retrieve one by one user's layout
bool create_layout(int code, short type, int enter_code)
{
	if (code == enter_code || type == RELEASED)
		return false;

	std::cout << "[ KEYCODE ]: " << code << std::endl;
	Key key{};
	key._code = code;

	// FIXME: Should do something else cause we have to type enter twice for the second input
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	key._normal = get_line_value("NORMAL");
	if (key._normal.empty())
		return true;

	key._alt = get_line_value("ALT");
	key._altgr = get_line_value("ALTGR");
	key._shift = get_line_value("SHIFT");
	key._capslock = get_line_value("CAPSLOCK");

	save_key(key);
	ebtask::log("Key added, type any and leave empty the NORMAL to stop or set is to continue");
	return false;
}

// To get and validate keys in config file
void load_keys()
{
	auto [key_path, key_json_config] = get_key_config();
	try
	{
		for (auto key : key_json_config["keys"])
		{
			Key new_key{};
			new_key._alt = key["alt"];
			new_key._normal = key["normal"];
			new_key._altgr = key["altgr"];
			new_key._shift = key["shift"];
			new_key._capslock = key["capslock"];
			_keys_.insert(std::make_pair(new_key._code, new_key));
		}
	}
	catch (json::exception error)
	{
		throw InvalidKeyError();
	}
}

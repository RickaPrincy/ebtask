#include <unordered_map>

#include "configuration.h"
#include "exception.h"
#include "types.h"
#include "utils.h"

using json = nlohmann::json;

std::unordered_map<int /*keycode*/, Key> _keys_{};

void load_keys()
{
	const auto &[config_keys_path, config_keys_content] =
		get_configuraton_content_with_path(EBTASK_KEYS_CONF_NAME, true);
	try
	{
		for (const auto &key : config_keys_content["keys"])
		{
			Key _new_key_{};
			_new_key_._code = key["code"];
			_new_key_._normal = key["normal"];
			_new_key_._altgr = key["_altgr"];
			_new_key_._shift = key["shift"];
			_new_key_._capslock = key["capslock"];
			_keys_.insert(std::make_pair(_new_key_._code, _new_key_));
		}
	}
	catch (json::exception error)
	{
		throw InvalidConfigurationError();
	}
}

void save_key(Key new_key)
{
	auto [config_keys_path, config_keys_content] =
		get_configuraton_content_with_path(EBTASK_KEYS_CONF_NAME);
	try
	{
		if (!config_keys_content.contains("keys"))
			config_keys_content["keys"] = json::array();
		else if (!config_keys_content["keys"].is_array())
			throw InvalidConfigurationError();
		json new_json_key = json::object({ { "code", new_key._code },
			{ "normal", new_key._normal },
			{ "altgr", new_key._altgr },
			{ "capslock", new_key._capslock },
			{ "shift", new_key._shift } });
		config_keys_content["keys"].push_back(new_json_key);

		save_json_file(config_keys_path, config_keys_content);
	}
	catch (json::exception error)
	{
		throw InvalidConfigurationError();
	}
	catch (std::exception error)
	{
		// TODO: when we cannot no longer open the file or creating the file
		throw InvalidConfigurationError();
	}
}

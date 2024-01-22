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


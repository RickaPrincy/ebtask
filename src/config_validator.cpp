#include <exception>
#include <iostream>

#include "config.h"
#include "utils/exception.h"

using json = nlohmann::json;

// make some huge validator here
Config get_config_if_valid(nlohmann::json config, std::string config_path)
{
	try
	{
		Config config_content{};
		config_content._path = config_path;

		for (int code : config["stop_keybinding"])
		{
			config_content._stop_keybinding.push_back(code);
		}

		for (auto _mode : config["modes"])
		{
			Mode mode{};
			for (int code : _mode["keybinding"])
			{
				mode._keybinding.push_back(code);
			}

			mode._name = _mode["name"];
			for (auto _action : _mode["actions"])
			{
				Action action{};
				for (int code : config["stop_keybinding"])
				{
					action._keybinding.push_back(code);
				}

				action._command = _action["command"];
				action._function = _action["function"];
				mode._actions.push_back(action);
			}
			config_content._modes.push_back(mode);
		}

		return config_content;
	}
	catch (nlohmann::json::exception error)
	{
		throw InvalidConfigurationError();
	}

	return Config();
}

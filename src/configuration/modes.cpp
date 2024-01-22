#include <vector>
#include "configuration.h"
#include "exception.h"
#include "json/json.hpp"
#include "types.h"
#include "utils.h"

using json = nlohmann::json;
static std::vector<Mode> _modes_{};

void load_modes()
{
	const auto &[config_path, config_content] =
		get_configuraton_content_with_path(EBTASK_CONF_NAME, true);

	try
	{
		for (const auto mode : config_content["modes"]){
            Mode _new_mode_{};
            _new_mode_._name = mode["name"];
            load_keybinding(mode, _new_mode_._keybinding);
            
            for(auto action: mode["actions"]){
                Action _new_action_{};
                _new_action_._command = action["command"];
                _new_action_._function = action["function"];
                load_keybinding(action, _new_action_._keybinding);
                _new_mode_._actions.push_back(_new_action_);
            }
            _modes_.push_back(_new_mode_);
        }
	}
	catch (json::exception error)
	{
		throw InvalidConfigurationError();
	}
}

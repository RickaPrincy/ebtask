#include <exception>
#include <iostream>

#include "config.h"
#include "utils/exception.h"

using json = nlohmann::json;

// make some huge validator here
Config get_config_if_valid(nlohmann::json config, std::string config_path)
{
    try{
        Config config_content{};
        config_content._path = config_path;
        config_content._stop_keybinding = config["stop_keybinding"];
        for(auto _mode : config["modes"]){
            Mode mode{};
            mode._keybinding =  _mode["keybinding"];
            for(auto _action: _mode["actions"]){
                Action action{};
                action._keybinding = _action["keybinding"];
                action._command = _action["command"];
                action._function = _action["function"];
                
                if(action._function.empty() && action._keybinding.empty()){
                    ebtask::exit_error("one of function and keybinding is required for each actions");
                }
                mode._actions.push_back(action);
            }
            config_content._modes.push_back(mode);
        }

        return config_content;
    }
    catch(nlohmann::json::exception error){
        throw InvalidConfigurationError();
    }

	return Config();
}

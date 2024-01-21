#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "command_actions.h"
#include "config.h"
#include "json/json.hpp"
#include "layout.h"
#include "utils/exception.h"
#include "utils/utils.h"

using json = nlohmann::json;

std::string get_line_value(std::string event)
{
	ebtask::log("Enter the value when " + event + " << is pressed");

	std::string value;
	std::getline(std::cin, value);
	std::cout << "\n";

	return value;
}

void save_key(const Key &key){
	std::string config_path = get_config_path();
    std::string keys_path = config_path + "/" + "ebtask.keys.json";
    std::ifstream current_keys(keys_path);

    json current_keys_json;

    if (current_keys.is_open()) {
        try {
		    current_keys_json = json::parse(current_keys);
        } catch (const std::exception& e) {
            current_keys.close();
            throw InvalidConfigurationError();
        }
        current_keys.close();
    }
    
    if(!current_keys_json.is_object())
        throw InvalidConfigurationError();

    if (!current_keys_json.contains("keys")) 
        current_keys_json["keys"] = json::array();
    
    try{
        json new_key = json::object();
        new_key["code"] = key._code;
        new_key["normal"] = key._normal;
        new_key["alt"] = key._alt;
        new_key["altgr"] = key._altgr;
        new_key["shift"] = key._shift;
        new_key["capslock"] = key._capslock;
        current_keys_json["keys"].push_back(new_key);
    }catch(json::exception error){
        throw InvalidConfigurationError();
    }

    std::ofstream current_keys_output(keys_path);
    if (current_keys_output.is_open()) {
        try {
            current_keys_output<< std::setw(4) << current_keys_json;
        } catch (const std::exception& e) {
            current_keys_output.close();
            throw NotFoundConfigurationError();
        }
        current_keys_output.close();
    } else {
        throw NotFoundConfigurationError();
    }
}

void create_layout(int code, short type)
{
    std::cout << "[ KEYMAP ]: " << code << std::endl;
	Key key{};
	key._code = code; 
    key._normal = get_line_value("NORMAL");
    key._alt = get_line_value("ALT");
    key._altgr = get_line_value("ALTGR");
    key._shift = get_line_value("SHIFT");
    key._capslock = get_line_value("CAPSLOCK");

    save_key(key);
    ebtask::log("Key added, presse another key to add or the stop keybinding or ctrl + c to stop");
}

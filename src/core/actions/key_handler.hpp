#pragma once

#include <string>

#include "../config/config.hpp"
#include "../os_inputs/os_inputs.hpp"

namespace ebtask
{
	void load_layout(std::string config_path);
	ebtask::KeyStatus update_key_status(int code, ebtask::KeyStatus status);
	ebtask::KeyStatus get_key_status(int code);
	bool is_all_pressed(const ebtask::KeyBinding &keybinding);
	bool is_backspace(int code);
	bool is_space(int code);
	bool is_special();
	std::string get_key_text_value(int code);

}  // namespace ebtask

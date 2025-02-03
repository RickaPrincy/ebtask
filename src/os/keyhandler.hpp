#pragma once

#include <string>

#include "../core/config/config.hpp"
#include "input.hpp"

namespace ebtask
{
	namespace keyhandler
	{
		void load_layout_config(std::string config_path);
		ebtask::os::input::KeyStatus update_key_status(int code, ebtask::os::input::KeyStatus status);
		ebtask::os::input::KeyStatus get_key_status(int code);
		bool is_all_pressed(const ebtask::config::KeyBinding &keybinding);
		bool is_last_key_pressed_is_special();
		bool is_backspace(int code);
		bool is_space(int code);
		std::string get_key_text_value(int code);
	}  // namespace keyhandler
}  // namespace ebtask

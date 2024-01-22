#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>

#include "command_actions.h"
#include "configuration/configuration.h"
#include "configuration/types.h"
#include "utils/logger.h"

static int _special_keys_recorded_ = 0;
static int _enter_code_ = 0;

static const std::array<std::string, 5> _special_keys_names_{ "ENTER",
	"CAPSLOCK",
	"LEFT_SHIFT",
	"RIRHT_SHIFT",
	"ALTGR" };

void save_special_key(int code)
{
	Key special_key{};
	std::string key_name = _special_keys_names_.at(_special_keys_recorded_);
	special_key._normal = special_key._shift = special_key._capslock = special_key._altgr =
		key_name;
	special_key._code = code;
	if (key_name == "ENTER")
		_enter_code_ = code;

	_special_keys_recorded_++;

	ELogger::log("Key code " + key_name + " is now " + std::to_string(code));
	save_key(special_key);
	ELogger::log("Key code " + key_name + " is now " + std::to_string(code));

	if (_special_keys_recorded_ < 5)
		ELogger::log("Now, Type " + _special_keys_names_.at(_special_keys_recorded_));
}

bool get_key_to_record(int code, KeyStatus status)
{
	if (status != KeyStatus::PRESSED)
		return false;

	if (_special_keys_recorded_ < _special_keys_names_.size())
	{
		save_special_key(code);
		return false;
	}

	return true;
}

// To retrieve one by one user's layout
ECallBack::ReadFunction remap_layout()
{
	ELogger::log("Remaping key started", false);
	ELogger::log("Ebtask need to know some special keys so let's go for it first", false);
	ELogger::log("First, Type the ENTER key");

	// Ask special key
	return get_key_to_record;
}

#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
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
	"RIGHT_SHIFT",
	"ALTGR" };

static std::string get_line(std::string name)
{
	std::cout << "[ LOG ]: Enter the value when " + name + " => ";
	std::string value;
	std::getline(std::cin, value);
	std::cout << "\n";

	return value;
}

// TODO: here is your last code bro (Have a nice day)
static bool ask_key_information(int code)
{
	ELogger::log("The Keycode is " + std::to_string(code));
	Key _new_key_{};
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	_new_key_._normal = get_line("NORMAL");
	if (_new_key_._normal.empty())
		return true;
	_new_key_._capslock = get_line("CAPSLOCK");
	_new_key_._shift = get_line("SHIFT (left or right)");
	_new_key_._altgr = get_line("ALTGR");
	_new_key_._code = code;
	save_key(_new_key_);

	ELogger::log("Key saved, type another key to save");
	return false;
}

static void save_special_key(int code)
{
	Key special_key{};
	std::string key_name = _special_keys_names_.at(_special_keys_recorded_);
	special_key._normal = key_name;
	special_key._shift = key_name;
	special_key._capslock = key_name;
	special_key._altgr = key_name;
	special_key._code = code;

	if (key_name == "ENTER")
		_enter_code_ = code;

	_special_keys_recorded_++;

	save_key(special_key);
	ELogger::log("Key code " + key_name + " is now " + std::to_string(code));

	if (_special_keys_recorded_ < 5)
		ELogger::log("Now, Type " + _special_keys_names_.at(_special_keys_recorded_));
	else if (_special_keys_recorded_ == 5)
	{
		ELogger::log("Now type any keys to record(avoid typing the same key twice)");
		ELogger::log("To stop recording, just leave a normal value empty");
	}
}

static bool get_key_to_record(int code, KeyStatus status, int &fd, const char *devnode)
{
	if (status != KeyStatus::PRESSED)
		return false;

	if (_special_keys_recorded_ < _special_keys_names_.size())
	{
		save_special_key(code);
		return false;
	}

	if (code == _enter_code_)
		return false;
	bool result = ask_key_information(code);
	close(fd);
	open(devnode, O_RDONLY);

	return result;
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

#include "keyhandler.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "../utils/fs_utils.hpp"
#include "../utils/json_utils.hpp"
#include "input.hpp"

static std::unordered_map<int /*keycode*/, ebtask::os::input::Key> _KEYS_{};

// to avoid a lot of query in the keys
static std::unordered_map<std::string /*keyname*/, ebtask::os::input::KeyStatus> _SPECIAL_KEYS_{
	{ "CAPSLOCK", ebtask::os::input::KeyStatus::RELEASED },
	{ "LEFT_SHIFT", ebtask::os::input::KeyStatus::RELEASED },
	{ "RIGHT_SHIFT", ebtask::os::input::KeyStatus::RELEASED },
	{ "ALTGR", ebtask::os::input::KeyStatus::RELEASED }
};

// some keys are quites specials but not really XD
static int _BACKSPACE_CODE_{ -1 }, _SPACE_CODE_{ -1 };
static bool _IS_LATEST_KEY_PRESSED_IS_SPECIAL_ = false;

void ebtask::keyhandler::load_layout_config(std::string config_path)
{
	nlohmann::json layout_content = ebtask::read_json_file(config_path, true);
	try
	{
		for (const auto &key : layout_content["keys"])
		{
			os::input::Key new_key{};
			READ_JSON_DATA(key, new_key, code);
			READ_JSON_DATA(key, new_key, normal);
			READ_JSON_DATA(key, new_key, altgr);
			READ_JSON_DATA(key, new_key, shift);
			READ_JSON_DATA(key, new_key, capslock);

			_KEYS_.insert(std::make_pair(new_key.code, new_key));

			if (new_key.normal == "BACKSPACE")
				_BACKSPACE_CODE_ = new_key.code;
			else if (new_key.normal == "SPACE")
				_SPACE_CODE_ = new_key.code;
		}
	}
	catch (nlohmann::json::exception error)
	{
		throw std::runtime_error("Make sure that your keys in ebtask keys are all valid");
	}
}

ebtask::os::input::KeyStatus ebtask::keyhandler::update_key_status(int code, ebtask::os::input::KeyStatus status)
{
	if (_KEYS_.find(code) == _KEYS_.end())
		return ebtask::os::input::KeyStatus::NOT_FOUND;

	ebtask::os::input::Key &key = _KEYS_[code];
	key.status = status;

	if (_SPECIAL_KEYS_.find(key.normal) != _SPECIAL_KEYS_.end())
	{
		_SPECIAL_KEYS_[key.normal] = status;
		_IS_LATEST_KEY_PRESSED_IS_SPECIAL_ = true;
	}
	else
		_IS_LATEST_KEY_PRESSED_IS_SPECIAL_ = false;
	return status;
}

ebtask::os::input::KeyStatus ebtask::keyhandler::get_key_status(int code)
{
	const auto &key = _KEYS_.find(code);
	if (key == _KEYS_.end())
		return ebtask::os::input::KeyStatus::NOT_FOUND;
	return key->second.status;
}

bool ebtask::keyhandler::is_all_pressed(const ebtask::config::KeyBinding &keybinding)
{
	return std::all_of(keybinding.begin(),
		keybinding.end(),
		[](const auto &value) { return get_key_status(value) == os::input::KeyStatus::PRESSED; });
}

bool ebtask::keyhandler::is_backspace(int code)
{
	return _BACKSPACE_CODE_ == code;
}

bool ebtask::keyhandler::is_space(int code)
{
	return _SPACE_CODE_ == code;
}

bool ebtask::keyhandler::is_last_key_pressed_is_special()
{
	return _IS_LATEST_KEY_PRESSED_IS_SPECIAL_;
}

// Used only after updating keys status and check if the key exist
std::string ebtask::keyhandler::get_key_text_value(int code)
{
	if (_SPECIAL_KEYS_["CAPSLOCK"] == ebtask::os::input::KeyStatus::PRESSED)
		return _KEYS_[code].capslock;
	else if (_SPECIAL_KEYS_["LEFT_SHIFT"] == ebtask::os::input::KeyStatus::PRESSED ||
			 _SPECIAL_KEYS_["RIGHT_SHIFT"] == ebtask::os::input::KeyStatus::PRESSED)
		return _KEYS_[code].shift;
	else if (_SPECIAL_KEYS_["ALTGR"] == ebtask::os::input::KeyStatus::PRESSED)
		return _KEYS_[code].altgr;
	return _KEYS_[code].normal;
}

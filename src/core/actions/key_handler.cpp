#include "key_handler.hpp"

#include <algorithm>
#include <json/json.hpp>

#include "../../utils/fs_utils.hpp"
#include "../../utils/json_utils.hpp"
#include "../os_inputs/os_inputs.hpp"

using json = nlohmann::json;
static std::unordered_map<int /*keycode*/, ebtask::Key> _KEYS_{};

// backspace is quite special but not really XD
static int _BACKSPACE_CODE_{ -1 }, _SPACE_CODE_{ -1 };
static bool _IS_LATEST_KEY_PRESSED_IS_SPECIAL_ = false;

// to avoid a lot of query in the keys
static std::unordered_map<std::string /*keyname*/, ebtask::KeyStatus> _SPECIAL_KEYS_{
	{ "CAPSLOCK", ebtask::KeyStatus::RELEASED },
	{ "LEFT_SHIFT", ebtask::KeyStatus::RELEASED },
	{ "RIGHT_SHIFT", ebtask::KeyStatus::RELEASED },
	{ "ALTGR", ebtask::KeyStatus::RELEASED }
};

void ebtask::load_layout(std::string config_path)
{
	json layout_content = ebtask::get_json_file_content(config_path, true);
	try
	{
		for (const auto &key : layout_content["keys"])
		{
			Key new_key{};
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
	catch (json::exception error)
	{
		throw std::runtime_error("Make sure that your keys in ebtask keys are all valid");
	}
}

ebtask::KeyStatus ebtask::update_key_status(int code, ebtask::KeyStatus status)
{
	if (_KEYS_.find(code) == _KEYS_.end())
		return ebtask::KeyStatus::NOT_FOUND;

	ebtask::Key &key = _KEYS_[code];
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

ebtask::KeyStatus ebtask::get_key_status(int code)
{
	const auto &key = _KEYS_.find(code);
	if (key == _KEYS_.end())
		return ebtask::KeyStatus::NOT_FOUND;
	return key->second.status;
}

bool ebtask::is_all_pressed(const ebtask::KeyBinding &keybinding)
{
	return std::all_of(keybinding.begin(),
		keybinding.end(),
		[](const auto &value) { return get_key_status(value) == KeyStatus::PRESSED; });
}

bool ebtask::is_backspace(int code)
{
	return _BACKSPACE_CODE_ == code;
}

bool ebtask::is_space(int code)
{
	return _SPACE_CODE_ == code;
}

bool ebtask::is_special()
{
	return _IS_LATEST_KEY_PRESSED_IS_SPECIAL_;
}

// Used only after updating keys status and check if the key exist
std::string ebtask::get_key_text_value(int code)
{
	if (_SPECIAL_KEYS_["CAPSLOCK"] == ebtask::KeyStatus::PRESSED)
		return _KEYS_[code].capslock;
	else if (_SPECIAL_KEYS_["LEFT_SHIFT"] == ebtask::KeyStatus::PRESSED ||
			 _SPECIAL_KEYS_["RIGHT_SHIFT"] == ebtask::KeyStatus::PRESSED)
		return _KEYS_[code].shift;
	else if (_SPECIAL_KEYS_["ALTGR"] == ebtask::KeyStatus::PRESSED)
		return _KEYS_[code].altgr;
	return _KEYS_[code].normal;
}

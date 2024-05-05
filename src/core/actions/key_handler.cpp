#include "key_handler.hpp"

#include <json/json.hpp>

#include "../../utils/fs_utils.hpp"
#include "../os_inputs/os_inputs.hpp"

using json = nlohmann::json;

static std::unordered_map<int /*keycode*/, ebtask::Key> _keys_{};

// backspace is quite special but not really xd
static int _BACKSPACE_CODE_{ -1 }, _SPACE_CODE_{ -1 };
static bool _IS_SPECIAL_ = false;

// to avoid a lot of query in the keys
static std::unordered_map<std::string /*keyname*/, ebtask::KeyStatus> _SPECIAL_KEYS_{
	{ "CAPSLOCK", ebtask::KeyStatus::RELEASED },
	{ "LEFT_SHIFT", ebtask::KeyStatus::RELEASED },
	{ "RIGHT_SHIFT", ebtask::KeyStatus::RELEASED },
	{ "ALTGR", ebtask::KeyStatus::RELEASED }
};

void ebtask::load_layout(std::string config_path)
{
	json layout_content = ebtask::get_json_file_content(config_path);
	try
	{
		for (const auto &key : layout_content["keys"])
		{
			Key _new_key_{};
			_new_key_._code = key["code"];
			_new_key_._normal = key["normal"];
			_new_key_._altgr = key["altgr"];
			_new_key_._shift = key["shift"];
			_new_key_._capslock = key["capslock"];
			_keys_.insert(std::make_pair(_new_key_._code, _new_key_));

			if (_new_key_._normal == "BACKSPACE")
				_BACKSPACE_CODE_ = _new_key_._code;
			else if (_new_key_._normal == "SPACE")
				_SPACE_CODE_ = _new_key_._code;
		}
	}
	catch (json::exception error)
	{
		throw std::runtime_error("Make sure that your keys in ebtask keys are all valid");
	}
}

ebtask::KeyStatus ebtask::update_key_status(int code, ebtask::KeyStatus status)
{
	if (_keys_.find(code) == _keys_.end())
		return ebtask::KeyStatus::NOT_FOUND;
	ebtask::Key &key = _keys_[code];
	key._status = status;
	std::string key_name = key._normal;

	if (_SPECIAL_KEYS_.find(key_name) != _SPECIAL_KEYS_.end())
	{
		_SPECIAL_KEYS_[key_name] = status;
		_IS_SPECIAL_ = true;
	}
	else
		_IS_SPECIAL_ = false;
	return status;
}

ebtask::KeyStatus ebtask::get_key_status(int code)
{
	if (_keys_.find(code) == _keys_.end())
		return ebtask::KeyStatus::NOT_FOUND;
	return _keys_[code]._status;
}

bool ebtask::is_all_pressed(const ebtask::KeyBinding &keybinding)
{
	for (const auto &keybinding_code : keybinding)
	{
		if (get_key_status(keybinding_code) != KeyStatus::PRESSED)
			return false;
	}
	return true;
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
	return _IS_SPECIAL_;
}

// Used only after updating keys status and check if the key exist
std::string ebtask::get_key_text_value(int code)
{
	if (_SPECIAL_KEYS_["CAPSLOCK"] == ebtask::KeyStatus::PRESSED)
		return _keys_[code]._capslock;
	else if (_SPECIAL_KEYS_["LEFT_SHIFT"] == ebtask::KeyStatus::PRESSED ||
			 _SPECIAL_KEYS_["RIGHT_SHIFT"] == ebtask::KeyStatus::PRESSED)
		return _keys_[code]._shift;
	else if (_SPECIAL_KEYS_["ALTGR"] == ebtask::KeyStatus::PRESSED)
		return _keys_[code]._altgr;
	return _keys_[code]._normal;
}

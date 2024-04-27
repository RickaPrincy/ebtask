#include <vector>

#include "json/json.hpp"
#include "types.h"

using json = nlohmann::json;

static std::vector<int> _normal_mode_keybinding_{};
static KeyStatus _capslock_code_{ KeyStatus::PRESSED }, _enter_code_{ KeyStatus::RELEASED },
	_altgr_code_{ KeyStatus::RELEASED }, _lshift_code_{ KeyStatus::RELEASED },
	_rshift_code_{ KeyStatus::RELEASED };

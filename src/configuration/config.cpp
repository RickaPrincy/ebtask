#include <vector>

#include "configuration.h"
#include "exception.h"
#include "types.h"
#include "utils.h"

using json = nlohmann::json;

static std::vector<int> _stop_keybinding_{};
static KeyStatus _capslock_code_{ KeyStatus::PRESSED}, _enter_code_{ KeyStatus::RELEASED },
	_altgr_code_{ KeyStatus::RELEASED }, _lshift_code_{ KeyStatus::RELEASED },
	_rshift_code_{ KeyStatus::RELEASED };

void load_configuration()
{
	const auto &[config_path, config_content] =
		get_configuraton_content_with_path(EBTASK_CONF_NAME, true);

	try
	{
		load_keybinding(config_content, _stop_keybinding_, "stop_keybinding");
	}
	catch (json::exception error)
	{
		throw InvalidConfigurationError();
	}
}

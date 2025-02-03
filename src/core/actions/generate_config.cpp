#include "../../utils/logger.hpp"
#include "../config/config.hpp"
#include "actions.hpp"

void ebtask::generate_config(std::string config_name, std::string error_action)
{
	std::string old_config_name = config_name;
	config_name = ebtask::config::EbtaskConfig::handle_config_file_already_exist_error(config_name, error_action);

	std::string config_path = ebtask::config::EbtaskConfig::append_config_base_path(config_name);
	const auto is_new_file = old_config_name == config_name;

	if (is_new_file)
	{
		ebtask::config::EbtaskConfig new_config = ebtask::config::EbtaskConfig::generate_new_config_template();
		new_config.save(config_path);
	}

	ebtask::log(config_path + " was genereated !!");
}

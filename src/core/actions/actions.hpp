#pragma once

#include <string>

#include "../../os/input.hpp"

namespace ebtask
{
	void generate_config(std::string config_name, std::string error_action);
	ebtask::os::input::ReaderFunction configure_layout(std::string layout_name, std::string error_action);
	ebtask::os::input::ReaderFunction listen_event(std::string layout_name, std::string config_name);

}  // namespace ebtask

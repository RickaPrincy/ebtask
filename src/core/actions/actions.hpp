#pragma once

#include <string>

#include "../os_inputs/os_inputs.hpp"

namespace ebtask
{
	ebtask::ReaderFunction remap_layout(std::string layout_name,
		std::string error_action);
	void generate_config(std::string config_name, std::string error_action);
	ebtask::ReaderFunction listen_event();

}  // namespace ebtask

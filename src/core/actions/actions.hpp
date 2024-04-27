#pragma once

#include <string>

#include "../os_inputs/os_inputs.hpp"

namespace ebtask
{
	ebtask::ReaderFunction remap_layout(std::string layout_name,
		std::string error_action = "ERROR");
	ebtask::ReaderFunction listen_event();

}  // namespace ebtask

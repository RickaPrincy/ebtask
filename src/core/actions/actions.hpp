#pragma once

#include <string>

#include "../../utils/fs_utils.hpp"
#include "../os_inputs/os_inputs.hpp"

namespace ebtask
{
	ebtask::PathExistErrorAction get_remap_error_action_value(std::string error_action_string);
	ebtask::ReaderFunction remap_layout(const std::string layout_name,
		ebtask::PathExistErrorAction action_on_exist = ebtask::PathExistErrorAction::ERROR);
	ebtask::ReaderFunction listen_event();

}  // namespace ebtask

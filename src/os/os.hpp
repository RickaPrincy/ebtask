#pragma once

#include <string>

#include "input.hpp"

namespace ebtask
{
	namespace os
	{
		bool execute_command(const std::string &command, bool log = true);
		void read_devnode_input_file(const char *devnode, ebtask::os::input::ReaderFunction callback);
	}  // namespace os
}  // namespace ebtask

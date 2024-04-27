#include <fcntl.h>
#include <unistd.h>

#include <filesystem>
#include <rcli/rcli.hpp>
#include <stdexcept>
#include <string>

#include "../../utils/logger.hpp"
#include "../config/config.hpp"
#include "actions.hpp"

namespace fs = std::filesystem;
static const rcli::InputConfig input_config = rcli::InputConfig().clean(true).required(true);

// To retrieve one by one user's layout
ebtask::ReaderFunction ebtask::remap_layout(const std::string layout_name,
	ebtask::PathExistErrorAction action_on_exist)
{
	std::string layout_filename = layout_name + ".layout.json";
	std::string layout_file_path = ebtask::get_config_file_path(layout_filename);

	if (fs::exists(layout_file_path))
	{
		if (!fs::is_regular_file(layout_file_path))
			throw std::runtime_error(layout_file_path + " is not a valid layout config file");

		// switch not working on enum class value
		if (action_on_exist == ebtask::PathExistErrorAction::ERROR)
			throw std::runtime_error(
				"Layout file already exists, use --error-action to specify what to do");
		else if (action_on_exist == ebtask::PathExistErrorAction::OVERRIDE)
			ebtask::warn("Your layout config will be overridden");
		else
		{
			layout_filename = ebtask::get_dumb_unique_prefix() + layout_filename;
			ebtask::copy_folder(layout_file_path, ebtask::get_config_file_path(layout_filename));
		}
	}

	return [](int code, ebtask::KeyStatus status, const int& fd, const char* devnode)
	{ std::cout << "status: " << std::endl; };
}

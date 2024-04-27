#include <fcntl.h>
#include <unistd.h>

#include <rcli/rcli.hpp>
#include <string>

#include "../config/config.hpp"
#include "actions.hpp"

static const rcli::InputConfig input_config = rcli::InputConfig().clean(true).required(true);

// To retrieve one by one user's layout
ebtask::ReaderFunction ebtask::remap_layout(std::string layout_name, std::string error_action)
{
	std::string layout_filename = layout_name + ".layout.json";
	layout_filename = ebtask::handle_config_file_already_exist_error(layout_name, error_action);

	return [](int code, ebtask::KeyStatus status, const int& fd, const char* devnode)
	{ std::cout << "status: " << std::endl; };
}

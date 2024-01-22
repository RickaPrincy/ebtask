#include <string>

#include "setup.h"
#include "utils/logger.h"

void start(ebtask::Callback callback, bool is_create_layout)
{
	std::string devnode = guess_input_file();
	if (devnode.empty())
	{
		ebtask::exit_error("Cannot use libudev.h");
	}

	ebtask::log("Use the following keyboard event path: " + devnode);

	if (!read_input_file(devnode.c_str(), callback, is_create_layout))
	{
		ebtask::exit_error(
			"Cannot open the file (probably a permission problem or missing keyboard");
	}

	ebtask::log("Finished with success !!");
}

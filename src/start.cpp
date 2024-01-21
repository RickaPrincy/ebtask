#include <string>

#include "setup.h"
#include "utils/utils.h"

void start(ebtask::Callback callback)
{
	std::string devnode = guess_input_file();
	if (devnode.empty())
	{
		ebtask::exit_error("Cannot use libudev.h");
	}

	ebtask::log("Use the following keyboard event path: " + devnode);

	if (!read_input_file(devnode.c_str(), callback))
	{
		ebtask::exit_error(
			"Cannot open the file (probably a permission problem or missing keyboard");
	}

	ebtask::log("Finished with success !!");
}

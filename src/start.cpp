#include <string>

#include "setup.h"
#include "utils/logger.h"

void start(ECallBack::StartFunction start_function)
{
	std::string devnode = guess_input_file();
	if (devnode.empty())
	{
		ELogger::exit_error("Cannot use libudev.h");
	}

	ELogger::log("Use the following keyboard event path: " + devnode);

	if (!read_input_file(devnode.c_str(), start_function()))
	{
		ELogger::exit_error(
			"Cannot open the file (probably a permission problem or missing keyboard)");
	}

	ELogger::log("Finished with success !!");
}

#include <string>

#include "configuration/exception.h"
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

	try
	{
		if (!read_input_file(devnode.c_str(), start_function()))
		{
			ELogger::log("Exit with error");
		}
		else
		{
			ELogger::log("Finished with success !!");
		}
	}
	catch (InvalidConfigurationError error)
	{
		ELogger::cerr(error.what());
	}
	catch (NotFoundConfigurationError error)
	{
		ELogger::cerr(error.what());
	}
}

#include "run.hpp"

#include "../utils/logger.hpp"
#include "os_inputs/os_inputs.hpp"

void ebtask::run(ebtask::RunnerFunction runner_function, std::string devnode)
{
	if (devnode.empty())
		devnode = ebtask::guess_input_file();
	ebtask::log("Use the following keyboard event path: " + devnode);
	ebtask::read_input_file(devnode.c_str(), runner_function());
	ebtask::log("Finished with success !!");
}

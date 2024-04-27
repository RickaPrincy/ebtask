#include "run.hpp"

#include "../utils/logger.hpp"
#include "os_inputs/os_inputs.hpp"

void ebtask::run(ebtask::RunnerFunction runner_function)
{
	ebtask::run(runner_function, ebtask::guess_input_file());
}

void ebtask::run(ebtask::RunnerFunction runner_function, std::string devnode)
{
	ebtask::log("Use the following keyboard event path: " + devnode);
	ebtask::read_input_file(devnode.c_str(), runner_function());
	ebtask::log("Finished with success !!");
}

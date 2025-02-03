#include "run.hpp"

#include "../os/os.hpp"
#include "../utils/logger.hpp"

void ebtask::run(ebtask::RunnerFunction runner_function, std::string devnode)
{
	ebtask::log("Use the following keyboard event path: " + devnode);
	ebtask::os::read_devnode_input_file(devnode.c_str(), runner_function());
	ebtask::log("Finished with success !!");
}

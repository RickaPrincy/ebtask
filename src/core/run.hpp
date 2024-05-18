#pragma once

#include <functional>

#include "os_inputs/os_inputs.hpp"

namespace ebtask
{
	using RunnerFunction = std::function<ebtask::ReaderFunction()>;
	void run(ebtask::RunnerFunction runner_funnction, std::string devnode = "");
}  // namespace ebtask

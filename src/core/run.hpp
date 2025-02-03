#pragma once

#include <functional>

#include "../os/input.hpp"

namespace ebtask
{
	using RunnerFunction = std::function<ebtask::os::input::ReaderFunction()>;
	void run(ebtask::RunnerFunction runner_funnction, std::string devnode = "");
}  // namespace ebtask

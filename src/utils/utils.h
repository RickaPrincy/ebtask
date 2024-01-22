#ifndef __EBTASK_UTILS__
#define __EBTASK_UTILS__

#include <cstdlib>
#include <iostream>
#include <string>

#define EBTASK_PATH_ENV "EBTASK_PATH"

enum class KeyStatus{
    NOT_FOUND = -1,
    PRESSED = 1,
    RELEASED = 0
};

namespace ebtask
{
	using Callback = bool(*)(int code, KeyStatus, int enter_code);

	inline void exit_error(std::string message)
	{
		std::cerr << "[ ERROR ]: " << message << std::endl;
		std::exit(EXIT_FAILURE);
	}

	inline void log(std::string message)
	{
		std::cout << "[ LOG ]: " << message << std::endl;
	}
};	// namespace ebtask

#endif

#ifndef __EBTASK_UTILS__
#define __EBTASK_UTILS__

#include <cstdlib>
#include <iostream>
#include <string>

#define PRESSED 1
#define RELEASED 0
#define EBTASK_PATH_ENV "EBTASK_PATH"

enum class StartType
{
	CREATE_LAYOUT,
	NORMAL
};

namespace ebtask
{
	using Callback = bool(*)(int code, short type, int enter_code);

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

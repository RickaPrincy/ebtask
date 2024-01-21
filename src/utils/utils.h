#ifndef __EBTASK_UTILS__
#define __EBTASK_UTILS__

#include <cstdlib>
#include <iostream>
#include <string>

#define PRESSED 0
#define RELEASED 1
#define EBTASK_PATH_ENV "EBTASK_PATH"

enum class StartType
{
	CREATE_LAYOUT,
	NORMAL
};

namespace ebtask
{
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

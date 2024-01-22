#ifndef __EBTASK_UTILS__
#define __EBTASK_UTILS__

#include <cstdlib>
#include <iostream>
#include <string>

namespace ELogger
{
	inline void exit_error(std::string message)
	{
		std::cerr << "[ ERROR ]: " << message << std::endl;
		std::exit(EXIT_FAILURE);
	}

	inline void log(std::string message, bool do_endl = true)
	{
		std::cout << "[ LOG ]: " << message;
		if (do_endl)
			std::cout << std::endl;
		else
			std::cout << "\n";
	}
};	// namespace ELogger

#endif

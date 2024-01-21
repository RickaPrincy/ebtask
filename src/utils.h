#ifndef __EBTASK_UTILS__
#define __EBTASK_UTILS__

#include <cstdlib>
#include <iostream>
#include <string>

namespace ebtask
{
	inline void exit_error(std::string message)
	{
		std::cerr << "[ ERROR ]: " << message << std::endl;
        std::exit(EXIT_FAILURE);
	}

    inline void log(std::string message){
		std::cout << "[ LOG ]: " << message << std::endl;
    }
};	// namespace ebtask

#endif

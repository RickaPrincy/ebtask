#pragma once

#include <iostream>
#include <string>

namespace ebtask
{
	inline void cerr(std::string message)
	{
		std::cerr << "[ ERROR ]: " << message << std::endl;
	}

	inline void log(std::string message, bool do_endl = true)
	{
		std::cout << "[ LOG ]: " << message;
		if (do_endl)
			std::cout << std::endl;
		else
			std::cout << "\n";
	}

	inline void warn(std::string message)
	{
		std::cout << "[ WARNING ]: " + message;
	}
};	// namespace ebtask

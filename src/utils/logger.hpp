#pragma once

#include <TColor/TColor.hpp>
#include <iostream>
#include <string>

namespace ebtask
{
	inline void cerr(std::string message)
	{
		std::cerr << TColor::B_RED << "[ ERR ]: " << TColor::B_WHITE << message << std::endl;
	}

	inline void log(std::string message, bool do_endl = true)
	{
		std::cout << TColor::B_GREEN << "[ LOG ]: " << TColor::B_WHITE << message;

		if (do_endl)
			std::cout << std::endl;
		else
			std::cout << "\n";
	}

	inline void warn(std::string message)
	{
		std::cout << TColor::B_YELLOW << "[ WAR ]: " << TColor::WHITE << message << std::endl;
	}
};	// namespace ebtask

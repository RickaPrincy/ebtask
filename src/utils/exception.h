#ifndef __EBTASK_EXCEPTION__
#define __EBTASK_EXCEPTION__

#include <exception>
#include <string>

#include "utils.h"

class InvalidConfigurationError : public std::exception
{
private:
	std::string message_;

public:
	InvalidConfigurationError() : message_("Your configuration is not valid, please read the docs")
	{
	}

	const char* what() const noexcept override
	{
		return message_.c_str();
	}
};

class NotFoundConfigurationError : public std::exception
{
private:
	std::string message_;

public:
	NotFoundConfigurationError()
	{
		std::string path_env_value = EBTASK_PATH_ENV;
		message_ = "Make sure that the " + path_env_value + " exist as an ENV on your machine";
	}
	const char* what() const noexcept override
	{
		return message_.c_str();
	}
};

#endif	// ! __EBTASK_EXCEPTION__

#ifndef __EBTASK_EXCEPTION__
#define __EBTASK_EXCEPTION__

#include <exception>
#include <string>

class InvalidConfigurationError : public std::exception
{
private:
	std::string message_;

public:
	InvalidConfigurationError(
		std::string message = "Your configuration is invalid, please read the docs")
		: message_("_INVALID_CONFIGURATION_ERROR_ => " + message)
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
	NotFoundConfigurationError(std::string message = "Make sure that EBTASK_PATH exist on your ENV")
		: message_("_NOT_FOUND_CONFIGURATION_ERROR_ => " + message)
	{
	}

	const char* what() const noexcept override
	{
		return message_.c_str();
	}
};

#endif	// ! __EBTASK_EXCEPTION__

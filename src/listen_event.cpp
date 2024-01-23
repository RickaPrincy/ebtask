#include <iostream>

#include "command_actions.h"
#include "configuration/types.h"

ECallBack::ReadFunction listen_event()
{
	return [](int code, KeyStatus status, int &fd, const char *devnode)
	{
		std::cout << "code from listen_event" << std::endl;
		return false;
	};
}

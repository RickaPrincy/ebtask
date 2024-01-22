#include <iostream>

#include "command_actions.h"

bool listen_event(int code, KeyStatus status)
{
	std::cout << "code from listen " << code << std::endl;
	return false;
}

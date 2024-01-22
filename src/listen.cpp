#include <iostream>

#include "command_actions.h"
#include "utils/utils.h"

bool listen(int code, KeyStatus status, int enter_code)
{
	std::cout << "code from listen " << code << std::endl;
	return false;
}

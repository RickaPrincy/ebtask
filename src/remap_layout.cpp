#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>

#include "command_actions.h"
#include "configuration/types.h"

// To retrieve one by one user's layout
ECallBack::ReadFunction remap_layout()
{
	return [](int code, KeyStatus status)
	{
		std::cout << "hello from create_layout" << std::endl;
		return true;
	};
}

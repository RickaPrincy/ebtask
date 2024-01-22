#ifndef __EBTASK_SETUP__
#define __EBTASK_SETUP__

#include <string>

#include "configuration/types.h"

void start(ECallBack::StartFunction start_function);

std::string guess_input_file();

bool read_input_file(const char *devnode, ECallBack::ReadFunction callback);

#endif	// !__EBTASK_SETUP__

#ifndef __EBTASK_SETUP__
#define __EBTASK_SETUP__

#include <string>

#include "utils/utils.h"

std::string guess_input_file();

bool read_input_file(const char *devnode, ebtask::Callback callback, bool is_create_layout = false);

#endif	// !__EBTASK_SETUP__

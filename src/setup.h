#ifndef __EBTASK_SETUP__
#define __EBTASK_SETUP__

#include <string>

#include "configuration/types.h"

void start(ebtask::Callback callback);

std::string guess_input_file();

bool read_input_file(const char *devnode, ebtask::Callback callback, bool is_create_layout);

#endif	// !__EBTASK_SETUP__

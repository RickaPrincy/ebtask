#ifndef __EBTASK_COMMAND_ACTION__
#define __EBTASK_COMMAND_ACTION__

#include "utils/utils.h"

bool stop(int code, short type);

bool create_layout(int code, KeyStatus status, int enter_code);
bool listen(int code, KeyStatus status, int enter_code);

#endif	// !__EBTASK_COMMAND_ACTION__

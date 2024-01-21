#ifndef __EBTASK_COMMAND_ACTION__
#define __EBTASK_COMMAND_ACTION__

bool stop(int code, short type);

bool create_layout(int code, short type, int enter_code);
bool listen(int code, short type, int enter_code);

#endif	// !__EBTASK_COMMAND_ACTION__

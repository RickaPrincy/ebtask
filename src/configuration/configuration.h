#ifndef __EBTASK_CONFIGURATION__
#define __EBTASK_CONFIGURATION__

#include <string>
#include "types.h"

#define EBTASK_KEYS_CONF_NAME "ebtask.keys.json"
#define EBTASK_CONF_NAME "ebtask.config.json"

void load_configuration();
void load_modes();
void load_keys();

void save_key(Key new_key);

#endif	// ! __EBTASK_CONFIGURATION__

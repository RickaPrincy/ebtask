#ifndef __EBTASK_CONFIGURATION__
#define __EBTASK_CONFIGURATION__

#include <string>

#include "types.h"

#define EBTASK_KEYS_CONF_NAME "ebtask.keys.json"
#define EBTASK_CONF_NAME "ebtask.config.json"

void load_modes();
void load_keys();

bool handle_mode();
void handle_function(int code);
void handle_action(int code);
void reset_handling();
KeyStatus update_key_status(int code, KeyStatus status);
KeyStatus get_key_status(int code);
std::string get_value(int code);
bool is_special();
void save_key(Key new_key);
bool is_all_pressed(const std::vector<int> &keybinding);
bool is_backspace(int code);
bool is_space(int code);

#endif	// ! __EBTASK_CONFIGURATION__

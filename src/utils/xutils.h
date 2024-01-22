#ifndef __EBTASK_GUESS_XUTILS__
#define __EBTASK_GUESS_XUTILS__

#include <string>

// SYSTEM COMMAND
std::string execute_command(const std::string &command);

// XCLIP
void copy_to_clipboard(const std::string &text);

// XDOTOOL
void do_backspace(int repeat);
void type_text_with_response(std::string text, int shortcut_lenth, bool is_error = false);

#endif	// !__EBTASK_GUESS_XUTILS__

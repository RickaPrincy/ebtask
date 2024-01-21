#ifndef __EBTASK_GUESS_XUTILS__
#define __EBTASK_GUESS_XUTILS__

#include <string>
std::string execute_command(const std::string &command);
void copy_to_clipboard(const std::string &text);
void do_backspace(int repeat);
void type_text_with_response(std::string text, int shortcut_lenth, bool is_error = false);

#endif	// !__EBTASK_GUESS_XUTILS__

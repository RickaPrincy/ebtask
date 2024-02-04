#ifndef __EBTASK_GUESS_XUTILS__
#define __EBTASK_GUESS_XUTILS__

#include <string>

// SYSTEM COMMAND
std::string execute_command(const std::string &command);

// XCLIP
void copy_to_clipboard(const std::string &text);
void execute_and_copy(const std::string &command);

// XDOTOOL
void do_backspace(int repeat);
void do_return();
void do_tab();

void execute_and_type(const std::string &command);
void type_text_with_response(std::string text, int shortcut_lenth, bool is_error = false);
void xtype_string(const std::string &text);

#endif	// !__EBTASK_GUESS_XUTILS__

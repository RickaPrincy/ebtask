#include "xutils.hpp"

#include <cstdlib>
#include <string>

#include "os.hpp"

void ebtask::copy_to_clipboard(const std::string &text)
{
	const std::string command = "echo \"" + text + "\" | xclip -selection clipboard";
	std::system(command.c_str());
}

void ebtask::xtype_string(const std::string &text)
{
	std::string command = "xdotool type \"" + text + "\"";
	ebtask::execute_command(command);
}

void ebtask::do_backspace(int repeat)
{
	if (repeat <= 0)
	{
		return;
	}
	std::string command =
		"xdotool key --clearmodifiers --repeat " + std::to_string(repeat) + " BackSpace";
	ebtask::execute_command(command);
}

void ebtask::execute_and_copy(const std::string &command)
{
	ebtask::copy_to_clipboard(execute_command(command));
}

void ebtask::execute_and_type(const std::string &command)
{
	ebtask::xtype_string(execute_command(command));
}

void ebtask::do_return()
{
	ebtask::execute_command("xdotool type --clearmodifiers Return");
}

void do_tab()
{
	ebtask::execute_command("xdotool type --clearmodifiers Tab");
}

#include "xutils.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

void copy_to_clipboard(const std::string &text)
{
	const std::string command = "echo \"" + text + "\" | xclip -selection clipboard";
	std::system(command.c_str());
}

std::string execute_command(const std::string &command)
{
	const char *cmd = command.c_str();
	// There is no bufferflow here XD
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

	if (!pipe)
	{
		throw std::runtime_error("Erreur lors de l'ouverture du pipe.");
	}

	while (!feof(pipe.get()))
	{
		if (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
		{
			result += buffer.data();
		}
	}
	return result;
}

void xtype_string(const std::string &text)
{
	std::string command = "xdotool type \"" + text + "\"";
	execute_command(command);
}

void do_backspace(int repeat)
{
	if (repeat <= 0)
	{
		return;
	}
	std::string command =
		"xdotool key --clearmodifiers --repeat " + std::to_string(repeat) + " BackSpace";
	execute_command(command);
}

void execute_and_copy(const std::string &command)
{
	copy_to_clipboard(execute_command(command));
}

void execute_and_type(const std::string &command)
{
	xtype_string(execute_command(command));
}

void do_return(){
  execute_command("xdotool type --clearmodifiers Return");
}

void do_tab(){
  execute_command("xdotool type --clearmodifiers Tab");
}

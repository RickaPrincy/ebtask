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
	// This will be trimed to avoid buffer overflow
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

void type_text_with_response(std::string text, int shortcut_size, bool is_error)
{
	do_backspace(shortcut_size);
	if (is_error)
		text = "[ EROOR ]: " + text;
	std::string command = "xdotool type \"" + text + "\"";
	execute_command(command);
}

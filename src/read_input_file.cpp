#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include <csignal>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "keymap.h"
#include "setup.h"
#include "start_listen.h"

// TO handle ctrl + c or something else that can stop the application
volatile std::sig_atomic_t is_running = true;

void signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		is_running = false;
	}
}

bool read_input_file(const char *devnode, StartType type)
{
	int fd = open(devnode, O_RDONLY);
	if (fd == -1)
		return false;

	struct input_event ev;

	// /!\ stopped manually with ctrl + c or keybinding
	while (is_running)
	{
		ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
		if (bytesRead == sizeof(struct input_event) && ev.type == EV_KEY && (ev.value == RELEASED || ev.value == PRESSED))
		{
			try
			{
				switch (type)
				{
					case StartType::NORMAL: start_listen(ev.code, ev.value); break;
					case StartType::CREATE_LAYOUT: create_layout(); break;
					default: break;
				}
			}
			catch (std::runtime_error error)
			{
				std::cerr << "[ ERROR ]: " << error.what() << std::endl;
				is_running = false;
			}
		}
	}

	close(fd);
	return true;
}

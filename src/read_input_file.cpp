#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include <csignal>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "configuration/exception.h"
#include "configuration/types.h"
#include "json/json.hpp"
#include "utils/logger.h"

// TO handle ctrl + c or something else that can stop the application
static bool is_running = true;

void signal_handler(int signal)
{
	is_running = false;
}

bool read_input_file(const char *devnode, ECallBack::ReadFunction callback)
{
	std::signal(SIGINT, signal_handler);

	struct input_event ev;
	int fd = open(devnode, O_RDONLY);

	if (fd == -1)
		return false;

	const short PRESSED_INT = static_cast<short>(KeyStatus::RELEASED);
	const short RELEASED_INT = static_cast<short>(KeyStatus::PRESSED);

	// /!\ stopped manually with ctrl + c or keybinding
	while (is_running)
	{
		ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
		if (bytesRead == sizeof(struct input_event) && ev.type == EV_KEY &&
			(ev.value == PRESSED_INT || ev.value == RELEASED_INT))
		{
			try
			{
				if (callback(ev.code, static_cast<KeyStatus>(ev.value)))
				{
					is_running = false;
					break;
				}
			}
			catch (InvalidConfigurationError error)
			{
				ELogger::cerr(error.what());
				break;
			}
			catch (NotFoundConfigurationError error)
			{
				ELogger::cerr(error.what());
				break;
			}
			// catch (std::exception error)
			// {
			// 	ELogger::cerr(error.what());
			// 	break;
			// }
		}
	}

	close(fd);
	return !is_running;
}

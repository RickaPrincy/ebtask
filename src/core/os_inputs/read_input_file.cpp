#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include <csignal>
#include <stdexcept>

#include "os_inputs.hpp"

// TO handle ctrl + c or something else that can stop the application
static bool is_running = true;

static void signal_handler(int signal)
{
	is_running = false;
}

void ebtask::read_input_file(const char *devnode, ebtask::ReaderFunction callback)
{
	std::signal(SIGINT, signal_handler);
	struct input_event ev;
	int fd = open(devnode, O_RDONLY);

	if (fd == -1)
		throw std::runtime_error("Cannot open devnode");

	// /!\ stopped manually with ctrl + c or keybinding
	while (is_running)
	{
		ssize_t bytes_read = read(fd, &ev, sizeof(struct input_event));
		
        if (bytes_read != sizeof(struct input_event) || ev.type != EV_KEY)
            continue;
        
        try
        {
            is_running = callback(ev.code, static_cast<KeyStatus>(ev.value), fd, devnode);
        }
        catch (const std::runtime_error &error)
        {
            close(fd);
            throw std::runtime_error(error);
        }
	}

	close(fd);
}

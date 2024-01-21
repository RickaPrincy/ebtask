#include "read_input_file.h"

#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include <iostream>

bool read_input_file(const char *devnode)
{
	int fd = open(devnode, O_RDONLY);
	if (fd == -1)
		return false;

	struct input_event ev;
	
    // /!\ STOPED USING SHORTCUT
	while (true)
	{
		ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
		if (bytesRead == sizeof(struct input_event) && ev.type == EV_KEY && (ev.value == 0 || ev.value == 1))
		{
			// TODO: Do something here depending on argv from main
		}
	}

	close(fd);
	return true;
}

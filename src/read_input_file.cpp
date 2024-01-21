#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include <csignal>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "command_actions.h"
#include "utils/exception.h"
#include "utils/utils.h"

// TO handle ctrl + c or something else that can stop the application
bool is_running = true;
void signal_handler(int signal)
{
	is_running = false;
}

bool read_input_file(const char *devnode, ebtask::Callback callback)
{
	std::signal(SIGINT, signal_handler);

	int fd = open(devnode, O_RDONLY);
	
    if (fd == -1)
		return false;

	struct input_event ev;
    bool is_first_key = true;
    int enter_code=0; 

	// /!\ stopped manually with ctrl + c or keybinding
	while (is_running)
	{
		ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
		if (bytesRead == sizeof(struct input_event) && ev.type == EV_KEY &&
			(ev.value == RELEASED || ev.value == PRESSED))
		{
            if(is_first_key){
                is_first_key = false;
                ebtask::log("Please, type ENTER first");
                continue;
            }
            
            if(enter_code == 0){
                enter_code = ev.code;
                ebtask::log("ENTER code is now " + std::to_string(ev.code) + " (restart ebtask if it's an error)");
                continue;
            }

			if (stop(ev.code, ev.value))
			{
				is_running = false;
				break;
			}

			try
			{
                if(callback(ev.code, ev.type, enter_code)){
                    is_running = false;
                    break;
                }
			}
			catch (NotFoundConfigurationError error)
			{
				std::cerr << "[ ERROR ]: " << error.what() << std::endl;
				break;
			}
			catch (InvalidConfigurationError error)
			{
				std::cerr << "[ ERROR ]: " << error.what() << std::endl;
				break;
			}
		}
	}

	close(fd);
	return !is_running;
}

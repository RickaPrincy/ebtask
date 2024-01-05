#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <iostream>
#include "ebtask/read_input_file.h"

bool read_input_file(const char *devnode) {
    int fd = open(devnode, O_RDONLY);
    if (fd == -1)
        return false;
    
    struct input_event ev;
    while (true) { // STOP USING SHORTCUT
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (
            bytesRead == sizeof(struct input_event) && 
            ev.type == EV_KEY &&
            (ev.value == 0 || ev.value == 1)
        ) {
            //TODO: Do something here depending on argv from main
        }
    }

    close(fd);
    return true;
}

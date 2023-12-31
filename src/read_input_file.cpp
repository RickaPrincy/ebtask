#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <iostream>
#include "read_input_file.h"
#include "map_code.h"

bool read_input_file(const char *devnode) {
    int fd = open(devnode, O_RDONLY);
    if (fd == -1)
        return false;
    
    struct input_event ev;
    bool is_esc_pressed = false;
    while (!is_esc_pressed) { //Stop if KEY_ESC if pressed
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (
            bytesRead == sizeof(struct input_event) && 
            ev.type == EV_KEY &&
            (ev.value == 0 || ev.value == 1)
        ) {
            is_esc_pressed = map_code(ev.code, ev.value) != MapSaveStatus::SUCCESS;
        }
    }

    close(fd);
    return true;
}
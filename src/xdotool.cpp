#include <cstdlib>
#include "xdotool.h"

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string execute_command(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        return "ERROR: Cannot launch the command";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

void do_backspace(int repeat){
    if(repeat <= 0){
        return;
    }
    std::string command = "xdotool key --clearmodifiers --repeat " + std::to_string(repeat) + " BackSpace";
    execute_command(command);
}

void type_response(int shortcut_size, std::string message,  bool is_error){
    do_backspace(shortcut_size);
    if(is_error)
        message = "EROOR: " + message;
    std::string command = "xdotool type \"" + message + "\"";
    execute_command(command);
}
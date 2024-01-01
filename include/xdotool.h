#ifndef __RKEYLOGGER_XX_XDOTOOL__
#define __RKEYLOGGER_XX_XDOTOOL__
    #include <string>
    std::string execute_command(const std::string &command);
    void do_backspace(int repeat = 1);
    void type_response(int shortcut_size, std::string message,  bool is_error = false);
#endif
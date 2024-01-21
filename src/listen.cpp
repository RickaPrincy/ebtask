#include <iostream>
#include "command_actions.h"

bool listen(int code, short type,int enter_code){
    std::cout << "code from listen " << code << std::endl;
    return false;
}

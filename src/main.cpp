#include "ebtask/ebstask.h"
#include <iostream>

int main (int argc, char *argv[]) {
    std::string devnode = guess_input_file();
    if(devnode.empty()){
        std::cerr << "[ ERROR ]: Cannot use libudev.h" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "[ LOG ]: Use the following keyboard event path: " << devnode << "\n";

    if(!read_input_file(devnode.c_str())){
        std::cout << "[ ERROR ]: Cannot open the file (probably a permission problem or missing keyboard)" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "[ LOG ]: Finished with success !!" << std::endl;
    return EXIT_SUCCESS;
    return 0;
}


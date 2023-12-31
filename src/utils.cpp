#include <chrono>
#include <ctime>
#include <iomanip>
#include "rkeylogger.h"

std::string get_current_datetime(){
    auto currentTime = std::chrono::system_clock::now();
    
    std::time_t time = std::chrono::system_clock::to_time_t(currentTime);
    std::tm localTime = *std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d_%H:%M:%S");

    return oss.str();
}
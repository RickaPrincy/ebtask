#include "event.h"
#include <unordered_map>
#include <iostream>
#include "xdotool.h"
#include "utils.h"

std::string current_value = "";
std::string function_name = "";
bool is_for_value = false;

//Use a json file to list all actions
std::unordered_map<std::string, std::string> actions{
    {"add", "add --input ?"}
};

void reset_event(){
    current_value = "";
    function_name = "";
    is_for_value = false;
}

ProcesssResponse check_event(std::string text){
    if(function_name.size() > 10 || current_value.size()> 10000){
        type_response(function_name.size() + current_value.size(), "Too long", true);
        return ProcesssResponse::END;
    }

    if(text == "("){
        if(is_for_value){
            type_response(function_name.size() + current_value.size() + 1 , "Check your function_name", true);
            return ProcesssResponse::END;
        }

        if(function_name.empty()){
            type_response(1, "No function name", true);
            return ProcesssResponse::END;
        }

        is_for_value = true;
        return ProcesssResponse::SUCCESS;
    }

    if(text == ")"){
        if(!is_for_value){
            type_response(1, "Value error", true);
            return ProcesssResponse::END;
        }
        
        auto key = actions.find(function_name);
        if(key == actions.end()){
            type_response(function_name.size() + current_value.size() + 1, "Function not found", true);
            return ProcesssResponse::END;
        }

        std::string command = actions.at(function_name);
        replace_value(command, current_value);

        type_response(current_value.size() + function_name.size(), execute_command(command));
        return ProcesssResponse::END;
    }

    if(is_for_value)
        current_value += text;
    else    
        function_name += text;

    std::cout << "current_value " <<  current_value << "\n";
    std::cout << "function_name " << function_name << "\n";
    return ProcesssResponse::SUCCESS;
}
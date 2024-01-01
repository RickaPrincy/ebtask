#include "event.h"
#include <unordered_map>
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
    is_for_value = true;
}

bool check_event(std::string text){
    if(function_name.size() > 50 || current_value.size()> 10000){
        type_response(function_name.size() + current_value.size(), "Too long", true);
        return false;
    }

    if(text == "("){
        if(is_for_value){
            type_response(1, "Using \"(\" as parameter is not possible", true);
            return false;
        }

        if(function_name.empty()){
            type_response(1, "No function name", true);
            return false;
        }

        is_for_value = true;
        return true;
    }

    if(text == ")"){
        if(!is_for_value){
            type_response(1, "Using \")\" as parameter is not possible", true);
        }
        
        auto key = actions.find(function_name);
        if(key == actions.end()){
            type_response(function_name.size() + current_value.size(), "Function not found", true);
        }

        std::string command = actions.at(function_name);
        replace_value(command, current_value);

        type_response(current_value.size() + function_name.size(), execute_command(command));
        return false;
    }

    if(is_for_value)
        current_value += text;
    else    
        function_name += function_name;

    return true;
}
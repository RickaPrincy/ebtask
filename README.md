# Ebtask: Execute your command in background 

Ebtask is a tool that can listen to your keyboard events `(like a keylogger`) and execute a command that you want based on key input.

What? Only that?? No, with Ebtask, you can pass parameters to the command to execute by using a string as a command trigger. So, it's not really a keymap, but more like a `copylot`.

Huh? So, what exactly??? :frowning:

## Getting started (hope you understand here) :walking:

You can use Ebtask for many reasons but here's one example to help you understand what Ebtask is:

#### [BadUsage but Simple Example]: Math Exam 

First, here is a part of ebtask config: 
```json
{                                                                        
    "command": "/home/johndoe/scripts/ask-chatgpt-and-copy-response.sh --input \"@input\",                                                                                        
    "function": "chatgpt"                                                                                                            
}                                                                                                                                  
```

So now, suppose you have a `math exam using Google Forms`, and you're not allowed to open `any other tabs`. :skull:

What can ebtask do for you? It allows you to type `chatgpt(what is 5 + 5) directly on your Google Form` (or anywhere else; ebtask only needs the key pressed event).  

Then, ebtask will take the parameter `what is 5 + 5` and execute this: `/home/johndoe/scripts/ask-chatgpt-and-copy-response.sh --input "@input"` by replacing **@input** with **what is 5 + 5**.

So this will be executed: `/home/johndoe/scripts/ask-chatgpt-and-copy-response.sh --input "what is 5 + 5"`.

**What about ChatGPT's response?** 

It's easy! Based on your system, adapt the script to copy the response or to write it directly onto your form like a copilot (uwu) using something like xdotool or similar tools

# Installation (no release yet) 

- Dependancies
    - CMake (Version 3.27 or later)
    - C++ Compiler with C++17 support
    - [rcli](https://github.com/RickaPrincy/rcli)

- Build manually

```bash
git clone https://github.com/RickaPrincy/ebtask.git

cd ebtask

mkdir build

cd build

cmake -S .. -B .

sudo make install

cd ../..

rm -rf ebtask 
```

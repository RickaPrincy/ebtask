# Ebtask: Execute Your Background Task (And Take Its Output Easily)

**ebtask**: execute your background task (and take read it's output easily)

Ebtask is a tool that can listen to your keyboard events (`like a keylogger`) and execute any command that you want based on key input (`like a keymap`). What? Only that?? No, with Ebtask, you can `pass parameters and use text as command triggers (like "copylot")`. So, what exactly ??? :anguished:

## Getting started (hope you understand here)

You can use Ebtask for many reasons. Here's one example to help you understand what Ebtask is:

[BadUsage but Simple Example]: Suppose you have an `math exam using Google Forms`, and you're `not allowed to open any other tabs`. On your form, you write `chatgpt(what is 5 + 5)`. Here, you ask ChatGPT with `what is 5 + 5` text directly on the Google Form, and you can receive the response directly on the form, eliminating the need for another tab.

How ?? :hushed:

You would write some code to fetch the ChatGPT API (in any language), make it a CLI application that Ebtask will call. 
So, you associate your CLI application with Ebtask actions (here, our action is `chatgpt(@input)`; more details on the Ebtask configuration later).

So, when you type chatgpt(something), the value of "something" will be passed to your CLI application. 
Then, in your code that fetches the ChatGPT API, you log just the response (using console.log() for example in JavaScript), and Ebtask will take it to another CLI specified in the output_reader section of the Ebtask configuration. 
The output_reader is simply a CLI that gives you access to the logged response to your form.

But how ?? 

But how??

It's easy. You could use, for instance, [xclip](https://github.com/astrand/xclip) to copy the response and paste it on your form. Or, even cooler, use [xdotool](https://github.com/jordansissel/xdotool) to directly write the response on the form.

# Installation (no release yet) 

- Dependancies
    - CMake (Version 3.27 or later)
    - C++ Compiler with C++17 support
    - libudev
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

# Configuration 

## Sep1:

## :warning: Ebtask needs to listen to your keyboard like a keylogger, so you must grant it access. Therefore, for security reasons, run Ebtask only when necessary.

- Now, give ebtask user acces to your keyboard 

```bash
sudo groupadd ebtask 
sudo usermod -aG ebtask your_username
sudo nano /etc/udev/rules.d/99-input.rules
SUBSYSTEM=="input", GROUP="ebtask", MODE="0640"
sudo udevadm control --reload-rules
sudo udevadm trigger
```
- Create directory for ebtask configuration and add the path of taht folder in env with name `EBTASK_PATH`
    - example: add this line in your `.zshrc (or .bashrc)`: 
    ```bash
        export EBTASK_PATH="/home/ricka/.config/ebtask"
    ```

## Step2:

- You can run `ebtask init_config` to generate new ebtask configuration (a file named `ebtask.config.json` should be created to your `EBTASK_PATH`) 
- ...

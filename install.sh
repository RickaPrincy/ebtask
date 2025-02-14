#!/bin/bash

LOG_FILE="ebtask-install.log"
echo "Installation started at $(date)" | tee -a $LOG_FILE

# Check if dependencies are installed
for dep in cmake make git; do
    if ! command -v $dep &>/dev/null; then
        echo "$dep is not installed. Please install it manually." | tee -a $LOG_FILE
        exit 1
    fi
done

echo "All dependencies are installed." | tee -a $LOG_FILE

# Check if tcolor is already installed
if yay -Qs tcolor &>/dev/null; then
    echo "tcolor is already installed." | tee -a $LOG_FILE
else
    # Check if yay is installed
    if command -v yay &>/dev/null; then
        echo "yay is installed." | tee -a $LOG_FILE
        yay -Sy tcolor | tee -a $LOG_FILE || exit 1
    else
        echo "Installing tcolor via script..." | tee -a $LOG_FILE
        bash <(curl -s https://raw.githubusercontent.com/RickaPrincy/TColor.hpp/main/install.sh) | tee -a $LOG_FILE || exit 1
    fi
fi

# Check if rcli is already installed
if yay -Qs rcli &>/dev/null; then
    echo "rcli is already installed." | tee -a $LOG_FILE
else
    # Install rcli
    if command -v yay &>/dev/null; then
        yay -Sy rcli | tee -a $LOG_FILE || exit 1
    else
        echo "Installing rcli via script..." | tee -a $LOG_FILE
        bash <(curl -s https://raw.githubusercontent.com/RickaPrincy/rcli/main/install.sh) | tee -a $LOG_FILE || exit 1
    fi
fi

# Clone and install ebtask
echo "Cloning ebtask repository..." | tee -a $LOG_FILE
git clone https://github.com/RickaPrincy/ebtask | tee -a $LOG_FILE || exit 1
cd ebtask || exit 1
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -S .. -B . | tee -a ../../$LOG_FILE || exit 1
sudo make install | tee -a ../../$LOG_FILE || exit 1

cd ../..
rm -rf ebtask
echo "ebtask installation completed." | tee -a $LOG_FILE

# Set LD_LIBRARY_PATH
SHELL_RC="$HOME/.bashrc"
[ -n "$ZSH_VERSION" ] && SHELL_RC="$HOME/.zshrc"

if ! grep -q "export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH" "$SHELL_RC"; then
    echo "export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH" >> "$SHELL_RC"
    echo "LD_LIBRARY_PATH added to $SHELL_RC" | tee -a $LOG_FILE
else
    echo "LD_LIBRARY_PATH is already configured." | tee -a $LOG_FILE
fi

echo "Installation successfully completed!" | tee -a $LOG_FILE

#!/bin/bash

EXECUTABLE_NAME="autosuggest"
SOURCE_FILE="auto_suggest.cpp"
INSTALL_DIR="/usr/local/bin"
CONFIG_FILE="$HOME/.zshrc"

echo "Compiling $SOURCE_FILE..."
g++ -std=c++11 -o $EXECUTABLE_NAME $SOURCE_FILE

if [ -f "$EXECUTABLE_NAME" ]; then
    echo "Moving $EXECUTABLE_NAME to $INSTALL_DIR..."
    sudo mv $EXECUTABLE_NAME $INSTALL_DIR
else
    echo "Compilation failed. Exiting..."
    exit 1
fi

echo "Adding alias to $CONFIG_FILE..."
if grep -q "alias $EXECUTABLE_NAME=" "$CONFIG_FILE"; then
    echo "Alias already exists in $CONFIG_FILE."
else
    echo "alias $EXECUTABLE_NAME='$INSTALL_DIR/$EXECUTABLE_NAME'" >> $CONFIG_FILE
    echo "Alias added to $CONFIG_FILE."
fi

echo "Sourcing $CONFIG_FILE..."
source $CONFIG_FILE

echo "Installation complete. You can now use '$EXECUTABLE_NAME' in your terminal."

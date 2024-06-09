#!/bin/bash

# This script receives a command and executes it in the terminal.

# Function to execute a command
execute_command() {
    local command="$1"
    echo "Executing command: $command"
    eval "$command"
}

# Main loop to receive and execute commands
while true; do
    echo "Enter a command to execute (or type 'exit' to quit):"
    read user_command
    if [ "$user_command" == "exit" ]; then
        echo "Exiting..."
        break
    else
        execute_command "$user_command"
    fi
done

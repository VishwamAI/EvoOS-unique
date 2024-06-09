#!/bin/bash

# This script automates tasks based on given prompts.

# Function to execute a command
execute_command() {
    local command="$1"
    echo "Executing command: $command"
    eval "$command"
}

# Function to detect the available browser
detect_browser() {
    if command -v google-chrome &> /dev/null; then
        echo "google-chrome"
    elif command -v microsoft-edge &> /dev/null; then
        echo "microsoft-edge"
    else
        echo "none"
    fi
}

# Function to perform a web search using the detected browser
perform_web_search() {
    local query="$1"
    local browser
    browser=$(detect_browser)
    if [ "$browser" == "none" ]; then
        echo "No supported browser found. Please install Google Chrome or Microsoft Edge."
    else
        echo "Performing web search for: $query"
        $browser "https://www.google.com/search?q=$query"
    fi
}

# Function to interpret a command using the NLP model
interpret_command() {
    local command="$1"
    export COMMAND="$command"
    /home/ubuntu/aios_venv/bin/python3 - <<END
import os
from textblob.classifiers import NaiveBayesClassifier

experience_utterances = ('how many years have you programming',
                         'how long have you been coding python',
                         'what languages do you know',
                         'what languages do you code',
                         'how long have you been programming',
                         'how long have you been coding',
                         'how long have you been developing',
                         'how long have you been writing code')

environment_utterances = ('what is your development environment',
                          'what is your dev environment',
                          'what is your setup',
                          'what is your coding environment',
                          'what is your programming environment',
                          'what is your development setup',
                          'what is your dev setup',
                          'what is your coding setup')

working_on_utterances = ('what are you working on',
                         'what are you making',
                         'what are you building',
                         'what are you coding',
                         'what are you developing',
                         'what are you writing',
                         'what are you creating',
                         'what are you doing')

search_utterances = ('search for',
                     'look up',
                     'find information on',
                     'google',
                     'search the web for')

system_info_utterances = ('what is the system information',
                          'show me the system info',
                          'display system information',
                          'system info',
                          'system information')

shell_command_utterances = ('run',
                            'execute',
                            'perform',
                            'do',
                            'echo',
                            'date',
                            'ls',
                            'pwd',
                            'cd',
                            'mkdir',
                            'rmdir',
                            'touch',
                            'rm',
                            'cp',
                            'mv')

experience_utterances = [(x, 'experience') for x in experience_utterances]
environment_utterances = [(x, 'environment') for x in environment_utterances]
working_on_utterances = [(x, 'working') for x in working_on_utterances]
search_utterances = [(x, 'search') for x in search_utterances]
system_info_utterances = [(x, 'system_info') for x in system_info_utterances]
shell_command_utterances = [(x, 'shell_command') for x in shell_command_utterances]

training_set = []
training_set.extend(experience_utterances)
training_set.extend(environment_utterances)
training_set.extend(working_on_utterances)
training_set.extend(search_utterances)
training_set.extend(system_info_utterances)
training_set.extend(shell_command_utterances)

classifier = NaiveBayesClassifier(training_set)

command = os.environ['COMMAND']
prob_dist = classifier.prob_classify(command)
category = prob_dist.max()
print(f"{category} {command}")
END
}

# Main loop to receive and execute commands
while true; do
    echo "Enter a command to execute (or type 'exit' to quit):"
    read user_command
    if [ "$user_command" == "exit" ]; then
        echo "Exiting..."
        break
    elif [[ "$user_command" == search* ]]; then
        search_query="${user_command#search }"
        perform_web_search "$search_query"
    else
        interpreted_command=$(interpret_command "$user_command")
        command_category=$(echo "$interpreted_command" | awk '{print $1}')
        command_to_execute=$(echo "$interpreted_command" | awk '{$1=""; sub(/^ /, ""); print}')
        case "$command_category" in
            "experience")
                echo "You asked about experience."
                ;;
            "environment")
                echo "You asked about the environment."
                ;;
            "working")
                echo "You asked about what I'm working on."
                ;;
            "search")
                search_query="${user_command#search }"
                perform_web_search "$search_query"
                ;;
            "system_info")
                echo "Displaying system information:"
                uname -a
                ;;
            "shell_command")
                execute_command "$command_to_execute"
                ;;
            *)
                echo "Unknown command category: $command_category"
                ;;
        esac
    fi
done

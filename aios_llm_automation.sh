#!/bin/bash

# This script automates tasks using a compressed language model from the transformers library.

# Function to execute a command
execute_command() {
    local command="$1"
    echo "Executing command: $command"
    eval "$command"
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

# Function to interpret and execute natural language prompts using a compressed language model
interpret_and_execute() {
    local prompt="$1"
    echo "Interpreting prompt: $prompt"
    source aios_venv/bin/activate
    python -c "
import sys
from transformers import pipeline

try:
    # Load a compressed language model
    model = pipeline('text-generation', model='distilgpt2')

    # Generate a response based on the prompt
    prompt = sys.argv[1]
    response = model(prompt, max_length=50, num_return_sequences=1, truncation=True)[0]['generated_text']

    # Print the response
    print(response)
except Exception as e:
    print(f'Error: {e}')
" "$prompt" > response.txt
    deactivate

    # Read the response from the file and execute it
    local response
    response=$(cat response.txt)
    echo "Generated response: $response"

    # Replace HTML entities with corresponding characters
    response=$(echo "$response" | sed 's/&quot;/"/g; s/&amp;/&/g; s/&lt;/</g; s/&gt;/>/g')

    # Check if the response is a valid command
    if [[ -n "$response" ]]; then
        # Execute only the first line of the response to avoid multiple commands
        first_line=$(echo "$response" | head -n 1)
        # Validate the first line to ensure it is a valid shell command
        if [[ "$first_line" =~ ^[a-zA-Z0-9_]+ ]]; then
            # Further validation to ensure the command is safe to execute
            if command -v "$(echo "$first_line" | awk '{print $1}')" &> /dev/null; then
                execute_command "$first_line"
            else
                echo "Invalid command generated: $first_line"
            fi
        else
            echo "Invalid command generated: $first_line"
        fi
    else
        echo "Invalid response generated: $response"
    fi

    # Clean up the response file
    rm response.txt
}

# Main execution
if [[ $# -eq 0 ]]; then
    echo "No command provided. Usage: ./aios_llm_automation.sh <command>"
    exit 1
fi

user_command="$1"
if [[ "$user_command" == search* ]]; then
    search_query="${user_command#search }"
    perform_web_search "$search_query"
else
    interpret_and_execute "$user_command"
fi

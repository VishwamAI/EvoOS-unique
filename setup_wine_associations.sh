#!/bin/bash

# Setup Wine associations for custom tools

# Function to create file associations
create_association() {
    local filetype=$1
    local command=$2

    echo "Creating association for $filetype files..."
    wine reg add "HKCU\Software\Classes\.$filetype" /ve /d "$filetype_auto_file" /f
    wine reg add "HKCU\Software\Classes\$filetype_auto_file" /ve /d "$filetype File" /f
    wine reg add "HKCU\Software\Classes\$filetype_auto_file\shell\open\command" /ve /d "$command" /f
}

# Create associations for .exe, .deb, .dmg, and .apk files
create_association "exe" "path\\to\\custom_tool_for_exe %1"
create_association "deb" "path\\to\\custom_tool_for_deb %1"
create_association "dmg" "path\\to\\custom_tool_for_dmg %1"
create_association "apk" "path\\to\\custom_tool_for_apk %1"

echo "Wine associations setup complete."

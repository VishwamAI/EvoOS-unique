# EvoOS Setup and Usage Instructions

## Introduction
This document provides instructions for setting up, configuring, and using the Artificial Intelligence Operating System (EvoOS). The EvoOS is designed to automate tasks based on given prompts, search the web using browsers like Google Chrome or Edge, update and code itself, and maintain user confidentiality.

## Prerequisites
- A machine with x86 architecture
- Ubuntu 22.04 LTS installed
- Internet connection

## Setting Up the Development Environment
1. **Install Essential Tools and Dependencies:**
   ```bash
   sudo apt-get update
   sudo apt-get install -y build-essential git curl python3 python3-pip
   ```

2. **Clone the Latest Stable Linux Kernel:**
   ```bash
   git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git
   cd linux
   git checkout v6.9.3
   ```

3. **Install AI Tools in a Virtual Environment:**
   ```bash
   python3 -m venv aios_venv
   source aios_venv/bin/activate
   pip install tensorflow torch transformers jupyter pandas scikit-learn nltk spacy
   deactivate
   ```

## Configuring the X Server
1. **Install Xorg and Xvfb:**
   ```bash
   sudo apt-get install -y xorg xvfb
   ```

2. **Start the Xvfb Server:**
   ```bash
   Xvfb :99 -screen 0 1600x1000x24 -nolisten tcp -auth /tmp/xvfb-run.CAVi8T/Xauthority &
   export DISPLAY=:99
   ```

3. **Configure X Server to Allow Local Connections:**
   ```bash
   export XAUTHORITY=/tmp/xvfb-run.CAVi8T/Xauthority
   xhost +local:
   ```

## Using the EvoOS Automation Scripts
1. **aios_llm_automation.sh:**
   This script automates tasks using a compressed language model. It takes a command as an argument, generates a response using the language model, validates that the response is a valid shell command, and then executes it.

   **Example Usage:**
   ```bash
   ./aios_llm_automation.sh "echo Test LLM automation"
   ```

2. **aios_task_automation.sh:**
   This script automates tasks based on given prompts. It can detect the available browser and perform web searches.

   **Example Usage:**
   ```bash
   ./aios_task_automation.sh
   ```

## Troubleshooting
- **X Server Authorization Issues:**
  If you encounter authorization issues with the X server, ensure that the `XAUTHORITY` environment variable is set correctly and that the `xhost` command is used to allow local connections.

  **Example:**
  ```bash
  export XAUTHORITY=/tmp/xvfb-run.CAVi8T/Xauthority
  xhost +local:
  ```

- **GPU Process Errors:**
  If you encounter errors related to the GPU process when running GUI applications, try running the application in headless mode or ensure that your environment has proper GPU support.

## Testing the EvoOS
To test the EvoOS for functionality and performance, perform a series of actions that simulate typical user interactions with the system. This includes executing commands through the `aios_llm_automation.sh` script, performing web searches, and opening GUI applications.

**Example Test:**
```bash
./aios_llm_automation.sh "google-chrome --no-sandbox https://www.google.com"
```

## Conclusion
By following these instructions, you should be able to set up, configure, and use the EvoOS effectively. If you encounter any issues, refer to the troubleshooting section for guidance.

# Boxedwine
Boxedwine is an emulator that runs Windows applications.  It achieves this by running a 32-bit version of Wine, and emulating the Linux kernel and CPU.  It is written in C++ with SDL and is supported on multiple platforms.

Boxedwine is open source and released under the terms of the GNU General Public License v2 (GPL).

## Features

- Runs 16/32-bit Windows applications
- Runs on Windows, Mac, Linux, Raspberry Pi and Web
- Can run multiple version of Wine, from 1.8 to 5.0
- Supports OpenGL and DirectX games.

## Needs Work

- Networking does not work well
- Mac OpenGL does not work with frame buffers
- ARMv8 CPU core for Mac M1 and Raspberry Pi still has bugs
- Games after the year 2000 have limited success at running

## Platforms that are tested

- Emscripten (WASM)
- Linux 64-bit
- Mac (Arm)
- Mac (Intel)
- Raspberry Pi 4 32-bit
- Raspberry Pi 4 64-bit
- Windows 32-bit
- Windows 64-bit


## Performance Test using MDK Perf with GDI backend on Wine

![This is an image](http://boxedwine.org/mdk.jpg)

Emscripten on Intel i7-6700K on Windows 10
- **27** Firefox 81
- **29** Chrome Version 96.0.4664.110 (Official Build) (64-bit)

Mac Mini M1
- **870** ARMv8 CPU Core
- **150** x64 CPU Core with Rosetta

iMac 2017 3.4GHz i5
- **245** x64 CPU Core

Raspberry Pi 4 64-bit
- **133** ARMv8 CPU Core

Raspberry Pi 4 32-bit
- **14** Normal CPU Core + JIT

Windows 10 on Intel i7-6700K
- **48** Normal CPU Core
- **64** Normal CPU Core + JIT
- **985** x64 CPU Core

Repo is at: https://github.com/danoon2/Boxedwine

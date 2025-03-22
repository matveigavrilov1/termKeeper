#!/bin/bash

#Conan default parameters
CONAN_HOST_PROFILE="windows-mingw-release"
CONAN_BUILD_PROFILE="linux-mingw-release"
CONAN_PROFILES_DIR="profiles"
CONAN_PROFILE_AUTODETECT=false

#CMake default parameters
CMAKE_GENERATOR="Ninja"
CMAKE_TARGET=""
CONFIGURE_ONLY=false
SRC_DIR="src"
BUILD_TYPE="Release"

#Python venv
VENV_DIR=".venv"

# Get the directory to set CONAN_HOME
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
CONAN_DIR="${SCRIPT_DIR}/.conan2"
export CONAN_HOME="$CONAN_DIR"

# Source the helper functions
source "$SCRIPT_DIR/tools/build-tools/functions.sh"
source "$SCRIPT_DIR/tools/build-tools/setup_venv.sh"
source "$SCRIPT_DIR/tools/build-tools/build_project.sh"

# Get available profiles and targets
get_available_profiles

# Parse command-line arguments
parse_args "$@"

# Set up Python virtual environment
setup_venv

# Build the project
build_project

deactivate

rm -f CMakeUserPresets.json
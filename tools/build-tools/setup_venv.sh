#!/bin/bash

setup_venv() {
	if [[ ! -d "$VENV_DIR" ]]; then
		echo "Creating Python virtual environment in '$VENV_DIR'..."
		python3 -m venv "$VENV_DIR"
	fi

	source "$VENV_DIR/bin/activate"

	echo "Installing Conan and other dependencies..."
	pip install conan ninja
	conan remote update conancenter --url="https://center2.conan.io"
}

#!/bin/bash

build_project() {
	export CONAN_HOME="$CONAN_DIR"
	CONAN_HOST_PROFILE_PATH="$CONAN_PROFILES_DIR/$CONAN_HOST_PROFILE"
	CONAN_BUILD_PROFILE_PATH="$CONAN_PROFILES_DIR/$CONAN_BUILD_PROFILE"

	if [[ ! " ${AVAILABLE_PROFILES[@]} " =~ " ${CONAN_HOST_PROFILE} " && "$CONAN_PROFILE_AUTODETECT" = false ]]; then
		echo "Error: Invalid profile '$CONAN_HOST_PROFILE'."
		echo "Available profiles:"
		for profile in "${AVAILABLE_PROFILES[@]}"; do
			echo "  - $profile"
		done
		exit 1
	fi

	if [[ ! " ${AVAILABLE_PROFILES[@]} " =~ " ${CONAN_BUILD_PROFILE} " && "$CONAN_PROFILE_AUTODETECT" = false ]]; then
		echo "Error: Invalid profile '$CONAN_BUILD_PROFILE'."
		echo "Available profiles:"
		for profile in "${AVAILABLE_PROFILES[@]}"; do
			echo "  - $profile"
		done
		exit 1
	fi

	if [[ "$CONAN_PROFILE_AUTODETECT" = true ]]; then
		conan profile detect
		conan install . --build=missing -r=conancenter -s build_type=$BUILD_TYPE \
			--output-folder="build"
	else
		conan install . --build=missing -r=conancenter -s build_type=$BUILD_TYPE \
			--profile:build="$CONAN_BUILD_PROFILE_PATH" --profile:host="$CONAN_HOST_PROFILE_PATH"\
			--output-folder="build"
	fi

	cd build

	cmake .. \
		-G $CMAKE_GENERATOR \
		-DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" \
		-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
		-DCMAKE_C_COMPILER=/usr/bin/x86_64-w64-mingw32-gcc \
		-DCMAKE_CXX_COMPILER=/usr/bin/x86_64-w64-mingw32-g++

	if [[ "$CONFIGURE_ONLY" = false ]]; then
		cmake --build . --config $BUILD_TYPE
		cmake --install . --config $BUILD_TYPE
	else
		echo "Project configuration complete. Skipping build."
	fi

	cd ..
}

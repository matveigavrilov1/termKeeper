#!/bin/bash

get_available_profiles() {
	if [[ -z "$CONAN_PROFILES_DIR" ]]; then
		echo "CONAN_PROFILES_DIR is not set.  Please set this environment variable." >&2
		return 1
	fi

	if [[ ! -d "$CONAN_PROFILES_DIR" ]]; then
		echo "Directory $CONAN_PROFILES_DIR does not exist." >&2
		return 1
	fi

	AVAILABLE_PROFILES=()
	for profile in "$CONAN_PROFILES_DIR"/*; do
		if [[ -f "$profile" ]]; then
			profile_name=$(basename "$profile")
			AVAILABLE_PROFILES+=("$profile_name")
		fi
	done

	AVAILABLE_PROFILES=($(printf '%s\n' "${AVAILABLE_PROFILES[@]}" | sort))
}

usage() {
	echo "Usage: $0 [options]"
	echo "Options:"
	echo "  --host-profile <profile>     Specify the Conan host profile (default: windows-mingw-release)"
	echo "  --build-profile <profile>    Specify the Conan build profile (default: linux-mingw-release)"
	echo "                               Available profiles:"
	for profile in "${AVAILABLE_PROFILES[@]}"; do
		echo "                               - $profile"
	done
	echo "  --profile-autodetect         Autodetect the Conan profile (be careful about C++ standard)"
	echo "  --generator <generator>      Specify the CMake generator (default: Ninja)"
	echo "  --configure-only             Only configure the project, do not build"
	echo "  --build-type <type>          Specify the build type (Debug, Release, etc., default: Release)"
	echo "  --help                       Display this help message"
	exit 1
}

parse_args() {
	while [[ $# -gt 0 ]]; do
		case $1 in
		--host-profile)
			CONAN_HOST_PROFILE="$2"
			shift 2
			;;
		--build-profile)
			CONAN_BUILD_PROFILE="$2"
			shift 2
			;;
		--profile-autodetect)
			CONAN_PROFILE_AUTODETECT=true
			shift
			;;
		--generator)
			CMAKE_GENERATOR="$2"
			shift 2
			while [[ $# -gt 0 && ! "$1" =~ ^-- ]]; do
				CMAKE_GENERATOR+=" $1"
				shift
			done
			;;
		--configure-only)
			CONFIGURE_ONLY=true
			shift
			;;
		--build-type)
			BUILD_TYPE="$2"
			shift 2
			;;
		--help)
			usage
			exit 0
			;;
		*)
			echo "Unknown argument: $1"
			usage
			exit 1
			;;
		esac
	done
}

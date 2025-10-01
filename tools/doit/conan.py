import os

from doit.tools import Interactive

def home_folder_arg(default=os.getcwd()):
	return {
		"name": "home_folder",
		"short": "h",
		"long": "home-folder",
		"type": str,
		"default": default,
		"help": "Home folder to work in"
	}

def output_folder_arg(default="build"):
	return {
		"name": "output_folder",
		"short": "o",
		"long": "output-folder",
		"type": str,
		"default": default,
		"help": "Output folder for conan"
	}

def conan_install():
	def conan_env(home_folder: str):
		return f"export CONAN_HOME={home_folder}/.conan2 &&"

	def configure_conan(home_folder: str):
		return f"{conan_env(home_folder)} conan remote update conancenter --url=\"https://center2.conan.io\""
	
	def conan_install(home_folder: str, output_folder: str):
		profile_options=f"--profile:build={home_folder}/tools/conan/build-profile --profile:host={home_folder}/tools/conan/host-profile"
		return f"{conan_env(home_folder)} conan install . --build=missing -r=conancenter {profile_options} --output-folder={home_folder}/{output_folder}"

	def out_dir_arg(default=True):
		return {
			"name": "out_dir",
			"short": "O",
			"long": "out-dir",
			"type": bool,
			"default": default,
			"help": "Remove \"out\" directory with artifacts"
		}

	return {
		'basename': "conan-install",
		'doc': "Install conan deps",
		'actions': [
			Interactive(configure_conan),
			Interactive(conan_install)
		],
		'verbosity': 2,
		'params': [
			home_folder_arg(),
			output_folder_arg()
		]
	}

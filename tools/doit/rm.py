import os
import shutil

def rm():
	def task(out_dir: bool):
		if out_dir:
			if os.path.exists("out"):
				shutil.rmtree("out")

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
		'basename': "rm",
		'doc': "Clear working space",
		'actions': [task],
		'verbosity': 2,
		'params': [out_dir_arg()]
	}

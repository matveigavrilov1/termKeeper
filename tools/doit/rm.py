import os
import shutil

from tools.doit.params import out_dir_arg

def rm():
	def task(out_dir: bool):
		if out_dir:
			if os.path.exists("out"):
				shutil.rmtree("out")

	return {
		'basename': "rm",
		'doc': "Clear working space",
		'actions': [task],
		'verbosity': 2,
		'params': [out_dir_arg()]
	}

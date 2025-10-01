def out_dir_arg(default=True):
	return {
		"name": "out_dir",
		"short": "O",
		"long": "out-dir",
		"type": bool,
		"default": default,
		"help": "Remove \"out\" directory with artifacts"
	}

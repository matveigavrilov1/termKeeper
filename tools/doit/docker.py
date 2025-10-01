from doit.tools import Interactive

def image_name_arg(default="termkeeperimage"):
		return {
			"name": "docker_image_name",
			"short": "n",
			"long": "docker-image-name",
			"type": str,
			"default": default,
			"help": "Name of docker image"
		}

def build_docker_image():
	def task(docker_image_name: str):
		cmd=f"sudo docker build --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) -t {docker_image_name} ."
		return cmd

	return {
		'basename': "build-docker-image",
		'doc': "Build docker image from Dockerfile",
		'actions': [Interactive(task)],
		'verbosity': 2,
		'params': [image_name_arg()]
	}

def build_project_with_docker():
	def task(docker_image_name: str):
		cmd=f"sudo docker run --rm -v $(pwd)/out:/out -v $(pwd):/termKeeper {docker_image_name}"
		return cmd
	
	return {
		'basename': "build",
		'doc': "Build project using image built from Docker file",
		'actions': [Interactive(task)],
		'verbosity': 2,
		'params': [image_name_arg()]
	}
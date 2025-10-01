from tools.doit.rm import rm
from tools.doit.docker import build_docker_image, build_project_with_docker

def task_build_docker_image():
    return build_docker_image()

def task_build_project_with_docker():
    return build_project_with_docker()

def task_rm():
	return rm()
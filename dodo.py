from tools.doit.docker import build_docker_image, build_project_with_docker
from tools.doit.conan import conan_install
from tools.doit.rm import rm

def task_build_docker_image():
    return build_docker_image()

def task_build_project_with_docker():
    return build_project_with_docker()

def task_conan_install():
	return conan_install()

def task_rm():
	return rm()
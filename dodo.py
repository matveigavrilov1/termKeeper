from tools.doit.rm import rm

def task_build_image():
    """Build Docker image"""
    return {
        'actions': ['sudo docker build --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) -t termkeeperimage .'],
        'file_dep': ['Dockerfile'],
        'verbosity': 2,
    }

def task_build():
    """Build project"""
    return {
        'actions': ['sudo docker run --rm -v $(pwd)/out:/out -v $(pwd):/termKeeper termkeeperimage'],
        'verbosity': 2,
    }

def task_rm():
	return rm()
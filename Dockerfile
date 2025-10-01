FROM ubuntu:24.04

ARG CMAKE_VERSION=3.28.3-1build7
ARG NINJA_VERSION=1.11.1-2
ARG MINGW_W64_VERSION=11.0.1-3build1
ARG POETRY_VERSION=1.8.2+dfsg-1ubuntu2

RUN apt-get update && apt-get install -y \
	cmake=${CMAKE_VERSION} \
	ninja-build=${NINJA_VERSION} \
	mingw-w64=${MINGW_W64_VERSION} \
	python3-poetry=${POETRY_VERSION} \
	bash \
	rsync \
	sudo

SHELL ["/bin/bash", "-c"]

ENV CC=x86_64-w64-mingw32-gcc
ENV CXX=x86_64-w64-mingw32-g++

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN usermod -u ${USER_ID} ubuntu && \
	groupmod -g ${GROUP_ID} ubuntu && \
	echo 'ubuntu ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers && \
	chown -R ubuntu:ubuntu /home/ubuntu

RUN mkdir -p /termKeeper && \
	mkdir -p /out && \
	chown -R ubuntu:ubuntu /out /termKeeper

USER ubuntu
WORKDIR /out

CMD sudo chown -R $(id -u):$(id -g) /out 2>/dev/null || true && \
	chmod -R 755 /out && \
	rsync -av --exclude-from='/termKeeper/.gitignore' /termKeeper/ . && \
	./tools/build/build.sh

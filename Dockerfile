# docker build -t termkeeperimage .
# docker run --rm -v $(pwd):/termKeeper termkeeperimage

FROM ubuntu:24.04

ARG CMAKE_VERSION=3.28.3-1build7
ARG NINJA_VERSION=1.11.1-2
ARG MINGW_W64_VERSION=11.0.1-3build1

RUN apt-get update && apt-get install -y \
    cmake=${CMAKE_VERSION} \
    ninja-build=${NINJA_VERSION} \
    mingw-w64=${MINGW_W64_VERSION}

ENV CC x86_64-w64-mingw32-gcc
ENV CXX x86_64-w64-mingw32-g++

RUN mkdir /termKeeper
WORKDIR /termKeeper

RUN mkdir build

CMD cmake -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/x86_64-w64-mingw32-g++ \
    -S . \
    -B build \
    -G Ninja && \
  cmake --build build --config Release --target all
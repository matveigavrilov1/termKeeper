cmake_minimum_required(VERSION 3.28.3)
project(project-controller VERSION 1.0.2 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static")

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_XOMPILER /usr/bin/x86_64-w64-mingw32-g++)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(git)

get_git_commit_hash(GIT_COMMIT_HASH)
message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")
add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

set(CMAKE_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/project-controller-${CMAKE_BUILD_TYPE}-${PROJECT_VERSION}-${GIT_COMMIT_HASH}")

find_package(pugixml REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(stduuid REQUIRED)

add_subdirectory(src)


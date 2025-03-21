cmake_minimum_required(VERSION 3.28.3)
project(termKeeper VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static")

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_XOMPILER /usr/bin/x86_64-w64-mingw32-g++)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

set(CMAKE_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/termKeeper-${CMAKE_BUILD_TYPE}")

find_package(pugixml REQUIRED)

add_subdirectory(src)

